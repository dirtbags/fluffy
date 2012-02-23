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
    fprintf(WHICHOUT(error), "Usage: %s [-x] MASK [MASK]*\n", prog);
    fprintf(WHICHOUT(error), "\t-x\tmask bytes are hexadecimal\n");
    fprintf(WHICHOUT(error), "\tMASK\ta mask byte\n");
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
    while ((opt = getopt(argc, argv, "hvx")) != -1) {
        switch (opt) {
            case 'x':
                base = 16;
                break;
            case 'v': return version(false);
            case 'h': return usage(false, argv[0]);
            default:  return usage(true,  argv[0]);
        }
    }

    if (optind >= argc) return usage(true, argv[0]);

    masklen = argc - optind;
    uint8_t mask[masklen];
    for (i = optind; i < argc; i++)
        mask[i-optind] = (uint8_t)strtol(argv[i], NULL, base);

    do_xor(mask, masklen);

    return EX_OK;
}
