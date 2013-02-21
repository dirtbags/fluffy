/*  ____    __   ____  ______
 * |    \  /  ] /    T|      T
 * |  o  )/  / Y  o  ||      |
 * |   _//  /  |     |l_j  l_j
 * |  | /   \_ |  _  |  |  |
 * |  | \     ||  |  |  |  |
 * l__j  \____jl__j__j  l__j
 *
 * dump a packet capture in a line-based, parsable format
 *
 */

#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sysexits.h>
#include "netre.h"
#include "pcap.h"
#include "stream.h"

#define IPPROTO_ICMP 1
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17
#define ARP_REQ 0x01
#define ARP_REP 0x02
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20

int version(bool error) {
    fprintf(WHICHOUT(error), "pcat v.%s - %s\n\n", PACKAGE_VERSION,
            "dump a packet capture in a line-based, parsable format");
    return error ? EX_USAGE : EX_OK;
}

int usage(bool error, char *prog) {
    int retval = version(error);
    fprintf(WHICHOUT(error), "Usage: %s [-o OUTPUT] [FILE]\n", prog);
    fprintf(WHICHOUT(error), "\t-o OUTPUT  file in which to write output; if not specified,\n");
    fprintf(WHICHOUT(error), "\t           output will be written to stdout\n");
    fprintf(WHICHOUT(error), "\tFILE       input pcap file\n");
    return retval;
}

void mac_addr(char *addr_s, uint16_t addr1, uint16_t addr2, uint16_t addr3) {
    snprintf(addr_s, 18, "%02x:%02x:%02x:%02x:%02x:%02x",
            (addr1 >> 8) & 0xff,
            (addr1 >> 0) & 0xff,
            (addr2 >> 8) & 0xff,
            (addr2 >> 0) & 0xff,
            (addr3 >> 8) & 0xff,
            (addr3 >> 0) & 0xff);
}

void ip4_addr(char *addr_s, uint32_t addr) {
    snprintf(addr_s, 16, "%u.%u.%u.%u",
            (addr >> 24) & 0xff,
            (addr >> 16) & 0xff,
            (addr >> 8)  & 0xff,
            (addr >> 0)  & 0xff);
}

void print_payload(struct stream *s, FILE *out) {
    if (s->len == 0)
        fprintf(out, "-");
    else
        while (s->len)
            fprintf(out, "%02x", read_uint8(s));
}

void process_icmp(struct stream *s, char *saddr_s, char *daddr_s, FILE *out) {
    uint8_t  type   = read_uint8(s);
    uint8_t  code   = read_uint8(s);
    uint16_t chksum = read_uint16be(s);

    fprintf(out, "ICMP4 %s,%u,%u %s ", saddr_s, type, code, daddr_s);

    if (false && chksum && false) {
        /* Placate compiler */
    }
}

void process_tcp(struct stream *s, char *saddr_s, char *daddr_s, FILE *out) {
    uint16_t sport  = read_uint16be(s);
    uint16_t dport  = read_uint16be(s);
    uint32_t seq    = read_uint32be(s);
    uint32_t ack    = read_uint32be(s);
    uint8_t  off    = read_uint8(s);
    uint8_t  hlen   = (off >> 4) * 4;
    uint8_t  flags  = read_uint8(s);
    uint16_t window = read_uint16be(s);
    uint16_t chksum = read_uint16be(s);
    uint16_t urgent = read_uint16be(s);

    if (hlen < 20) printf("!");

    fprintf(out, "TCP4 %s,%u,%u %s,%u,%u ", saddr_s, sport, seq, daddr_s, dport, ack);

    if (false && urgent && chksum && window && flags && ack && seq && false) {
        /* Placate compiler */
    }
}

void process_udp(struct stream *s, char *saddr_s, char *daddr_s, FILE *out) {
    uint16_t sport  = read_uint16be(s);
    uint16_t dport  = read_uint16be(s);
    uint16_t len    = read_uint16be(s);
    uint16_t chksum = read_uint16be(s);

    fprintf(out, "UDP4 %s,%u %s,%u ", saddr_s, sport, daddr_s, dport);

    if (false && len && chksum && false) {
        /* Placate compiler */
    }
}

void process_arp(struct stream *s, FILE *out) {
    uint16_t htype  = read_uint16be(s);
    uint16_t ptype  = read_uint16be(s);
    uint8_t  hlen   = read_uint8(s);
    uint8_t  plen   = read_uint8(s);
    uint16_t op     = read_uint16be(s);

    uint16_t sha1   = read_uint16be(s);
    uint16_t sha2   = read_uint16be(s);
    uint16_t sha3   = read_uint16be(s);
    uint32_t spa    = read_uint32be(s);

    uint16_t tha1   = read_uint16be(s);
    uint16_t tha2   = read_uint16be(s);
    uint16_t tha3   = read_uint16be(s);
    uint32_t tpa    = read_uint32be(s);

    // 12:34:56:78:9A:BC
    char sha_s[20];
    char spa_s[20];
    char tha_s[20];
    char tpa_s[20];

    mac_addr(sha_s, sha1, sha2, sha3);
    ip4_addr(spa_s, spa);
    mac_addr(tha_s, tha1, tha2, tha3);
    ip4_addr(tpa_s, tpa);

    switch(op) {
        case ARP_REQ:
            fprintf(out, "ARP? "); break;
        case ARP_REP:
            fprintf(out, "ARP. "); break;
        default:
            fprintf(out, "ARP! "); break;
    }

    fprintf(out, "%s,%s %s,%s -", sha_s, spa_s, tha_s, tpa_s);

    if (false && htype && ptype && hlen && plen && false) {
        /* Placate compiler */
    }
}

void process_ip4(struct stream *s, FILE *out) {
    uint8_t  vhl    = read_uint8(s);
    uint8_t  ihl    = (vhl & 0x0f) * 4;
    if (ihl < 20) ihl = 20;
    uint8_t  tos    = read_uint8(s);
    uint16_t length = read_uint16be(s);
    uint16_t id     = read_uint16be(s);
    uint16_t off    = read_uint16be(s);
    uint8_t  ttl    = read_uint8(s);
    uint8_t  proto  = read_uint8(s);
    uint16_t chksum = read_uint16be(s);
    uint32_t saddr  = read_uint32be(s);
    uint32_t daddr  = read_uint32be(s);

    char saddr_s[20];
    char daddr_s[20];

    ip4_addr(saddr_s, saddr);
    ip4_addr(daddr_s, daddr);

    DDUMP_d(ihl);
    DDUMP_d(length);

    // Ignore options
    sskip(s, ihl - 20);

    // Force stream length to IP payload length
    s->len = length - ihl;

    DDUMP_d(proto);
    switch (proto) {
        case IPPROTO_ICMP:
            process_icmp(s, saddr_s, daddr_s, out); break;
        case IPPROTO_TCP:
            process_tcp( s, saddr_s, daddr_s, out); break;
        case IPPROTO_UDP:
            process_udp( s, saddr_s, daddr_s, out); break;
        default:
            fprintf(out, "P%d %s %s ", proto, saddr_s, daddr_s); break;
    }

    print_payload(s, out);

    if (false && chksum && id && tos && ttl && off && false) {
        /* Placate compiler */
    }
}

void print_ethernet(struct stream *s, FILE *out) {
    uint8_t  saddr[6];
    uint8_t  daddr[6];
    uint16_t vlan;
    uint16_t ethertype;

    sread(s, &saddr, sizeof(saddr));
    sread(s, &daddr, sizeof(daddr));
    ethertype = read_uint16be(s);

    if (ethertype == 0x8100) { // VLAN
        vlan = read_uint16be(s);
        ethertype = read_uint16be(s);
    }

    switch (ethertype) {
        case 0x0806:          // ARP
            process_arp(s, out); break;
        case 0x0800:          // IPv4
        default:
            DDUMP_x(ethertype);
            process_ip4(s, out); break;
    }

    if (false && vlan && false) {
        /* Placate compiler */
    }
}

void print_frame(struct pcap_pkthdr *hdr, char const *frame, int raw, FILE *out) {
    struct stream streambuf;
    struct stream *s = &streambuf;

    sinit(s, frame, hdr->caplen);
    fprintf(out, "%u.%6.6u ", hdr->ts.tv_sec, hdr->ts.tv_usec);

    if (raw)
        process_ip4(s, out);
    else
        print_ethernet(s, out);

    fprintf(out, "\n");
}

int pcat(FILE *f, FILE *out) {
    struct pcap_file p;

    if (pcap_open_in(&p, f) == -1) {
        return EX_IOERR;
    }

    for (;;) {
        struct pcap_pkthdr hdr;
        char frame[MAXFRAME];

        if (pcap_read_pkthdr(&p, &hdr)     == -1) break;
        if (fread(frame, hdr.caplen, 1, f) !=  1) break;

        print_frame(&hdr, frame, p.raw, out);
    }

    return EX_OK;
}

int main(int argc, char *argv[]) {
    int opt;
    FILE *output = stdout;
    FILE *input  = stdin;

    while ((opt = getopt(argc, argv, "hvo:")) != -1) {
        switch (opt) {
            case 'o': /* output file */
                output = fopen(optarg, "wb");
                if (!output) {
                    perror("opening output");
                    return EX_OSFILE;
                }
                break;
            case 'v': return version(false);
            case 'h': return usage(false, argv[0]);
            default:  return usage(true,  argv[0]);
        }
    }

    if (optind < argc) {
        input = fopen(argv[optind], "rb");
        if (!input) {
            perror("opening input");
            return EX_OSFILE;
        }
    }

    return pcat(input, output);
}
