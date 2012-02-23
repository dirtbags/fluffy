/*                    _
 *  _ __  _   _ _ __ (_) __ _
 * | '_ \| | | | '_ \| |/ _` |
 * | |_) | |_| | | | | | (_| |
 * | .__/ \__,_|_| |_|_|\__, |
 * |_|                     |_|
 *
 * filter one to many PCAPs for unique frames
 *
 */

#include <getopt.h>
#include <stdio.h>
#include <stdbool.h>
#include <sysexits.h>
#include <string.h>
#include "netre.h"
#include "pcap.h"

int version(bool error) {
    fprintf(WHICHOUT(error), "puniq v.%s - %s\n\n", PACKAGE_VERSION,
            "filter one to many PCAPs for unique frames");
    return error ? EX_USAGE : EX_OK;
}

int usage(bool error, char *prog) {
    int retval = version(error);
    fprintf(WHICHOUT(error), "Usage: %s [-o OUTPUT] <FILE> [FILE]*\n", prog);
    fprintf(WHICHOUT(error), "\t-o OUTPUT\tfile in which to write output; if not specified, output will be written to stdout\n");
    fprintf(WHICHOUT(error), "\tFILE\tinput pcap file, at least one file is required\n");
    return retval;
}

int puniq(FILE *output, char *inputs[], int ninputs) {
    int                i;
    struct pcap_pkthdr hdr[2];
    char               frame[2][MAXFRAME];
    struct pcap_file   out;
    int                cur;

    if (pcap_open_out(&out, output) == -1) {
        perror("writing header");
        return EX_IOERR;
    }

    for (i = 0; i < ninputs; i++) {
        char             *fn = inputs[i];
        FILE             *f;
        struct pcap_file  p;

        if ((! fn) || (strcmp("-", fn) == 0)) {
            f = stdin;
        } else {
            printf("processing: %s\n", fn);
            f = fopen(fn, "rb");
            if (NULL == f) {
                perror(fn);
                return EX_IOERR;
            }
        }

        if (pcap_open_in(&p, f) == -1) {
            fprintf(stderr, "unable to process: %s\n", fn);
            return EX_IOERR;
        }

        cur = 0;
        while (true) {

            memset(&hdr[cur], 0, sizeof(hdr[cur]));
            memset(&frame[cur], 0, MAXFRAME);

            if (pcap_read_pkthdr(&p, &hdr[cur])             == -1) break;
            if (fread(frame[cur], hdr[cur].caplen, 1, p.f)  !=  1) break;
            if ((memcmp(&hdr[0], &hdr[1], sizeof(hdr[cur])) ==  0)  &&
               (memcmp(frame[0], frame[1], hdr[cur].caplen) ==  0)) {
                /* Skip this duplicate */
                DDUMP("duplicate!");
            } else {
                if (pcap_write_pkthdr(&out, &hdr[cur]) == -1) break;
                if (fwrite(frame[cur], hdr[cur].caplen, 1, out.f) != 1) break;
            }

            cur = (1 - cur);
        }

        pcap_close(&p);
    }
    pcap_close(&out);

    return EX_OK;
}

int main (int argc, char *argv[]) {
    int opt;
    FILE *output = stdout;

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

    /* at least one file is required */
    if (argc <= optind) return usage(true, argv[0]);

    return puniq(output, &argv[optind], argc - optind);
}
