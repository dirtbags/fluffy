/*
 *        _  _             _ _ _
 *  _ __ | || |  ___ _ __ | (_) |_
 * | '_ \| || |_/ __| '_ \| | | __|
 * | |_) |__   _\__ \ |_) | | | |_
 * | .__/   |_| |___/ .__/|_|_|\__|
 * |_|              |_|
 *
 * split a pcap into multiple files, based on a CIDR filter
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sysexits.h>
#include <strings.h>
#include <getopt.h>
#include "pcap.h"
#include "netre.h"

/* Gaah so much crap */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/in.h>

int version(bool error) {
    fprintf(WHICHOUT(error), "p4split v.%s - %s\n\n", PACKAGE_VERSION,
            "Splits PCAP into up to 256 files based on CIDR");
    return error ? EX_USAGE : EX_OK;
}

int usage(bool error, char *prog) {
    int retval = version(error);
    fprintf(WHICHOUT(error), "Usage: %s [-i INPUT] <CIDR>\n", prog);
    fprintf(WHICHOUT(error), "\t-i INPUT\tA PCAP to read for input\n");
    fprintf(WHICHOUT(error), "\tCIDR\t\tA network address in CIDR notation\n");
    fprintf(WHICHOUT(error), "\nif INPUT is not specified, stdin will be read\n");
    return retval;
}


int parse_cidr(char *s, uint32_t *addr, uint8_t *bits) {
    char           *slash = index(s, '/');
    struct in_addr  inp;

    if (slash != NULL) {
        *slash = '\0';
        *bits = (uint8_t)atoi(slash + 1);
    } else {
        *bits = 0;
    }

    if (inet_aton(s, &inp) == 0) return -1;
    *addr = ntohl(inp.s_addr);

    return 0;
}

int p4split(struct pcap_file input, uint32_t addr, uint8_t bits) {
    int              i;
    struct pcap_file out[256];
    int              errid = 0;
    uint32_t         mask  = ~((1 << (32-bits)) - 1);
    uint8_t          shr   = (32-bits) - 8;

    addr &= mask;

    for (i = 0; i < 256; i += 1) out[i].f = NULL;

    while (true) {
        struct pcap_pkthdr hdr;
        uint8_t            octet;
        char               frame[MAXFRAME];

        if (pcap_read_pkthdr(&input, &hdr)       == -1) break;
        if (fread(frame, hdr.caplen, 1, input.f) !=  1) break;

        {
            struct ether_header *eh = (struct ether_header *)frame;
            struct iphdr        *ih = (struct iphdr *)(frame + sizeof(struct ether_header));
            uint32_t             a;

            /* VLAN tag */
            if (ntohs(eh->ether_type) == 0x8100) {
                ih = (struct iphdr *)((char *)ih + 4);
            }

            a = ntohl(ih->saddr);
            if ((a & mask) != addr) {
                a = ntohl(ih->daddr);
                if ((a & mask) != addr) {
                    fprintf(stderr,
                            "Warning: dropping unmatched packet %08x -> %08x\n",
                            ntohl(ih->saddr), ntohl(ih->daddr));
                    continue;
                }
            }
            octet = (a & ~mask) >> shr;
        }

        if (! out[octet].f) {
            char fn[9];
            FILE *f;

            snprintf(fn, 9, "%d.pcap", octet);

            if (((f = fopen(fn, "wb"))         == NULL) ||
                (pcap_open_out(&out[octet], f) ==   -1)) {
                perror("Error creating output");
                errid = EX_CANTCREAT;
                break;
            }
        }

        if ((pcap_write_pkthdr(&out[octet], &hdr)       == -1) ||
            (fwrite(frame, hdr.caplen, 1, out[octet].f) !=  1)) {
            perror("Error writing");
            errid = EX_IOERR;
            break;
        }
    }

    for (i = 0; i < 256; i++) if (out[i].f) pcap_close(&out[i]);

    pcap_close(&input);

    return errid;
}

int main(int argc, char *argv[]) {
    int              opt;
    struct pcap_file p;
    uint32_t         addr;
    uint8_t          bits;
    FILE            *input;

    while ((opt = getopt(argc, argv, "hvi:")) != -1) {
        switch (opt) {
            case 'i': /* input */
                input = fopen(optarg, "rb");
                if (!input) {
                    perror("open input");
                    return EX_IOERR;
                }
                break;
            case 'v': return version(false);
            case 'h': return usage(false, argv[0]);
            default:  return usage(true,  argv[0]);
        }
    }

    if ((optind >= argc) || (argc - optind != 1)) {
        fprintf(stderr, "not enough arguments\n");
        return usage(true, argv[0]);
    }

    if (parse_cidr(argv[optind], &addr, &bits) == -1) {
        fprintf(stderr, "invalid CIDR\n");
        return EX_USAGE;
    }

    if (bits > 24) {
        fprintf(stderr, "subnet bits must be <= 24\n");
        return EX_USAGE;
    }

    if (pcap_open_in(&p, input) == -1) return EX_IOERR;

    if (bits % 8)
        fprintf(stderr, "Warning: routing prefix is not a multiple of 8.\n");

    return p4split(p, addr, bits);
}
