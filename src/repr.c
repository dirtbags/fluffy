/*
 *   _ __ ___ _ __  _ __
 *  | '__/ _ \ '_ \| '__|
 *  | | |  __/ |_) | |
 *  |_|  \___| .__/|_|
 *           |_|
 *
 * Quote non-printable bytes, similar to python's repr()
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include <sysexits.h>
#include <getopt.h>
#include "netre.h"

int version(bool error) {
    fprintf(WHICHOUT(error), "repr v.%s - %s\n\n", PACKAGE_VERSION,
            "quote non-printable bytes, similar to python's repr()");
    return error ? EX_USAGE : EX_OK;
}

int usage(bool error, char *prog) {
    int retval = version(error);
    fprintf(WHICHOUT(error), "Usage: %s [filename]\n\n", prog);
    fprintf(WHICHOUT(error), "if a file is not specified, stdin will be read\n");
    return retval;
}

int repr(FILE *f) {
    int c;

    while (EOF != (c = getc(f))) {
        switch (c) {
            case EOF: return 0;
            case 134:
                printf("\\\\");
                break;
            case 32 ... 91:
            case 93 ... 126:
                putchar(c);
                break;
            default:
                printf("\\x%02x", c);
                break;
        }
    }

    printf("\n");
    return 0;
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
            perror("opening input file");
            return EX_OSFILE;
        }
    }
    return repr(input);
}
