/*
 *  _ __  _ __ ___   ___ _ __ __ _  ___
 * | '_ \| '_ ` _ \ / _ \ '__/ _` |/ _ \
 * | |_) | | | | | |  __/ | | (_| |  __/
 * | .__/|_| |_| |_|\___|_|  \__, |\___|
 * |_|                       |___/
 *
 * Merges pcap files, outputting time-ordered pcap stream
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sysexits.h>
#include <getopt.h>
#include "pcap.h"
#include "netre.h"

struct input_file {
    int                active;
    struct pcap_file   p;
    struct pcap_pkthdr next;
};

struct linklayer {
    uint16_t           src[3];
    uint16_t           dest[3];
    uint16_t           ethertype;
};

int version(bool error) {
    fprintf(WHICHOUT(error), "pmerge v.%s - %s\n\n", PACKAGE_VERSION,
            "Merges pcap files, outputting time-ordered pcap stream");
    return error ? EX_USAGE : EX_OK;
}

int usage(bool error, char *prog) {
    int retval = version(error);
    fprintf(WHICHOUT(error), "Usage: %s [-o OUTPUT] FILE [FILE]*\n", prog);
    fprintf(WHICHOUT(error), "\tOUTPUT\toutput filename -- if not specified, stdout will be used\n");
    fprintf(WHICHOUT(error), "\tFILE\tinput filename, at least one is required\n");
    return retval;
}

int read_next(struct input_file *file) {
    if (! file->active) return -1;

    if (pcap_read_pkthdr(&file->p, &file->next) == -1) {
        pcap_close(&file->p);
        file->active = 0;
        file->next.ts.tv_sec = 0xffffffff;
        file->next.ts.tv_usec = 0xffffffff;
        return -1;
    }

    return 0;
}

int pmerge(FILE *output, struct input_file files[], int nfiles) {
    int               nopen;
    int               i;
    struct pcap_file  out;
    struct linklayer  fake = {{0,0,0}, {1,1,1}, 0x0008};

    if (pcap_open_out(&out, output) == -1) {
        perror("writing pcap header");
        return EX_IOERR;
    }

    nopen = nfiles;
    while (nopen) {
        struct input_file *cur = &files[0];
        char               frame[MAXFRAME];
        size_t             len;

        /* Find next oldest frame */
        for (i = 0; i < nfiles; i += 1) {
            if ( files[i].active                                     &&
               ((files[i].next.ts.tv_sec <  cur->next.ts.tv_sec)     ||
               ((files[i].next.ts.tv_sec == cur->next.ts.tv_sec)     &&
               ( files[i].next.ts.tv_usec <  cur->next.ts.tv_usec)))) {
               cur = &files[i];
            }
        }

        /* Make sure it'll fit */
        if (cur->next.caplen > sizeof(frame)) {
            fprintf(stderr, "error: huge frame (size %u)\n", (unsigned int)len);
            return EX_SOFTWARE;
        }

        /* Read it */
        len = fread(frame, 1, cur->next.caplen, cur->p.f);
        if (len < cur->next.caplen) {
            /* Short read */
            cur->next.caplen = len;
            pcap_close(&cur->p);
            cur->active = 0;
        }

        /* Write header + frame */
        if (len) {
            if (cur->p.raw) {
                cur->next.caplen += sizeof(fake);
                cur->next.len += sizeof(fake);
            }
            if (fwrite(&cur->next, sizeof(cur->next), 1, output) != 1) {
                perror("error");
                return EX_IOERR;
            }
            if (cur->p.raw) {
                if (fwrite(&fake, sizeof(fake), 1, output) != 1) {
                    perror("error");
                    return EX_IOERR;
                }
            }
            if (len != fwrite(frame, 1, len, output)) {
                perror("error");
                return EX_IOERR;
            }
        }

        if (read_next(cur) == -1) nopen -= 1;
    }

    return EX_OK;
}

int main(int argc, char *argv[]) {
    int  opt;
    FILE *output = stdout;

    while ((opt = getopt(argc, argv, "hvo:")) != -1) {
        switch (opt) {
            case 'o': /* output file */
                output = fopen(optarg, "wb");
                if (!output) {
                    perror("Open output");
                    return EX_OSFILE;
                }
                break;
            case 'v': return version(false);
            case 'h': return usage(false, argv[0]);
            default:  return usage(true,  argv[0]);
        }
    }

    if (optind >= argc) return usage(true, argv[0]);

    int               i;
    int               nfiles = 0;
    struct input_file files[argc - optind];

    /* Open input files */
    for (i = optind; i < argc; i++) {
        char              *fn  = argv[i];
        struct input_file *cur = &files[nfiles];
        FILE              *f;

        if (fn[0] == '-') return usage(true, argv[0]);

        f = fopen(fn, "rb");
        if (!f) {
            perror(fn);
            return EX_NOINPUT;
        }

        /* prime the input files */
        if (pcap_open_in(&cur->p, f) == -1) {
            fprintf(stderr, "%s: unable to process\n", fn);
            return EX_IOERR;
        }
        cur->active = 1;

        if (read_next(cur) == 0) nfiles += 1;
    }

    return pmerge(output, files, nfiles);
}
