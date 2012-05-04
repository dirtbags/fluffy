/*
 *                    __ _ _ _
 * __  _____  _ __   / _(_) | |_ ___ _ __
 * \ \/ / _ \| '__| | |_| | | __/ _ \ '__|
 *  >  < (_) | |    |  _| | | ||  __/ |
 * /_/\_\___/|_|    |_| |_|_|\__\___|_|
 *
 * apply mask bytes to the pipeline
 *
 */

#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sysexits.h>
#include <getopt.h>
#include "netre.h"

int version(bool error) {
    fprintf(WHICHOUT(error), "xor v.%s - %s\n\n", PACKAGE_VERSION,
            "apply mask bytes to the pipeline, using XOR");
    return error ? EX_USAGE : EX_OK;
}

int usage(bool error, char *prog) {
    int retval = version(error);
    fprintf(WHICHOUT(error), "Usage:\t%s [-x] MASK [MASK]*\n", prog);
    fprintf(WHICHOUT(error), "\t%s  -s  MASKSTRING\n", prog);
    fprintf(WHICHOUT(error), "\t-x\tmask bytes are hexadecimal\n");
    fprintf(WHICHOUT(error), "\t-s\tuse a string of characters as a mask\n");
    fprintf(WHICHOUT(error), "\n\tMASK\t\ta mask byte\n");
    fprintf(WHICHOUT(error), "\tMASKSTRING\ta string of mask characters\n");
    return retval;
}

void do_xor(uint8_t mask[], int len) {
    int i = 0;

    while (true) {
        int c = getchar();

        if (EOF == c) break;

        c ^= mask[i];

        putchar(c);
        i = (i + 1) % len;
    }
}

int main(int argc, char *argv[]) {
    int opt;
    int masklen = 0;
    int base    = 0;
    int i;

    /* option parsing */
    while ((opt = getopt(argc, argv, "xsvh")) != -1) {
        switch (opt) {
            case 'x':       /* hex  */
                base = 16;
                break;
            case 's':       /* string */
                base = -1;
                break;
            case 'v': return version(false);
            case 'h': return usage(false, argv[0]);
            default:  return usage(true,  argv[0]);
        }
    }

    if (optind >= argc) return usage(true, argv[0]);

    if (base != -1) {
        masklen = argc - optind;
        uint8_t mask[masklen];
        for (i = optind; i < argc; i++)
            mask[i-optind] = (uint8_t)strtol(argv[i], NULL, base);

        do_xor(mask, masklen);
    } else { /* string */
        masklen = strlen(argv[optind]);
        uint8_t mask[masklen];
        for (i = 0; i < masklen; i++)
            mask[i] = (uint8_t)argv[optind][i];

        do_xor(mask, masklen);
    }

    return EX_OK;
}
