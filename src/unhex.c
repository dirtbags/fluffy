/*
 *               _
 *   _   _ _ __ | |__   _____  __
 *  | | | | '_ \| '_ \ / _ \ \/ /
 *  | |_| | | | | | | |  __/>  <
 *   \__,_|_| |_|_| |_|\___/_/\_\
 *
 * convert hexits to their binary equivalent
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <sysexits.h>
#include <getopt.h>
#include "netre.h"

int version(bool error) {
    fprintf(WHICHOUT(error), "unhex v.%s - %s\n\n", PACKAGE_VERSION,
            "convert hexits to their binary equivalent");
    return error ? EX_USAGE : EX_OK;
}

int usage(bool error, char *prog) {
    int retval = version(error);
    fprintf(WHICHOUT(error), "Usage: %s [filename]\n\n", prog);
    fprintf(WHICHOUT(error), "if a file is not specified, stdin will be read\n");
    return retval;
}

int unhex(FILE *f) {
    unsigned char     acc    = '\0';
    unsigned char     nybble = '\0';
    unsigned long int count  = 0;

    while (true) {
        int c = getc(f);

        count += 1;

        switch (c) {
            case EOF: return EX_OK;
            case '0' ... '9':
                acc = (acc << 4) + c - '0';
                nybble += 1;
                break;
            case 'a' ... 'f':
                acc = (acc << 4) + c - 'a' + 10;
                nybble += 1;
                break;
            case 'A' ... 'F':
                acc = (acc << 4) + c - 'A' + 10;
                nybble += 1;
                break;
            default:
                if (nybble != 0)
                    fprintf(stderr,
                            "Warning: non-hex character mid-octet at offset %lu\n", count);
                break;
        }

        if (nybble == 2) {
            putchar(acc);
            acc = 0;
            nybble = 0;
        }
    }

    return EX_OK;
}

int main(int argc, char *argv[]) {
    int opt;
    FILE *input = stdin;

    /* option parsing */
    while ((opt = getopt(argc, argv, "hv")) != -1) {
        switch (opt) {
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

    return unhex(input);
}
