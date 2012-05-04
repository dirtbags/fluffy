/*
 *   _                        __
 *  | |_ _ __ __ _ _ __  ___ / _| ___  _ __ _ __ ___   ___ _ __
 *  | __| '__/ _` | '_ \/ __| |_ / _ \| '__| '_ ` _ \ / _ \ '__|
 *  | |_| | | (_| | | | \__ \  _| (_) | |  | | | | | |  __/ |
 *   \__|_|  \__,_|_| |_|___/_|  \___/|_|  |_| |_| |_|\___|_|
 *
 * apply bytes to the pipeline
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

typedef enum transform {
    xor,
    rot,
    rol,
    caesar
} transform_t;

static const char *ttoa(transform_t p) {
    switch(p) {
        case xor: return "xor"; break;
        case rot: return "rot"; break;
        case rol: return "rol"; break;
        case caesar: return "caesar"; break;
        default : return "---"; break;
    }
}

transform_t parse_tname(char *arg) {
    char *t = basename(arg);

    if (!strncasecmp(t, "rot", 3)) return rot;
    if (!strncasecmp(t, "rol", 3)) return rol;
    if (!strncasecmp(t, "caesar", 6)) return caesar;
    return xor;
}

int version(bool error, transform_t t) {
    fprintf(WHICHOUT(error), "%s v.%s - %s %s\n\n", ttoa(t), PACKAGE_VERSION,
            "apply bytes to the pipeline, using", ttoa(t));
    return error ? EX_USAGE : EX_OK;
}

int usage(bool error, transform_t t) {
    int retval = version(error, t);
    fprintf(WHICHOUT(error), "Usage:\t%s [-x] BYTE [BYTE]*\n", ttoa(t));
    fprintf(WHICHOUT(error), "\t%s -s  BYTESTRING\n", ttoa(t));
    fprintf(WHICHOUT(error), "\t-s\tuse a string of characters as the byte source\n");
    fprintf(WHICHOUT(error), "\t-x\tbytes are hexadecimal\n");
    if (t != xor)
        fprintf(WHICHOUT(error), "\t-u\tundo %s\n", ttoa(t));
    return retval;
}

#define XOR(c,s)          (c ^ s)
#define ROTL(c,s)         ((c >> (s%8)) | (c << (8-(s%8)))) & 0xFF
#define ROTR(c,s)         ((c << (s%8)) | (c >> (8-(s%8)))) & 0xFF
#define ROT(d,c,s)        (d ? ROTR(c,s) : ROTL(c,s))
#define ROLL(c,s)         (c - s) % 256
#define ROLR(c,s)         (c + s) % 256
#define ROL(d,c,s)        (d ? ROLR(c,s) : ROLL(c,s))
#define CAESARE(c,s,a)    ((c - a + s) % 26) + a
#define CAESARD(c,s,a)    ((c - a - s) % 26) + a
#define CAESAR(d,c,s,a)   (d ? CAESARD(c,s,a) : CAESARE(c,s,a))

int CAE(int undo, int c, uint8_t s) {
    if (isupper(c)) return CAESAR(undo, c, s, 'A');
    if (islower(c)) return CAESAR(undo, c, s, 'a');
    return c;
}

int action(transform_t t, uint8_t mask[], int len, bool undo) {
    int i = 0;

    while (true) {
        int c = getchar();
        if (c == EOF) break;

        switch(t) {
            case rot:    c = ROT(undo, c, mask[i]); break;
            case rol:    c = ROL(undo, c, mask[i]); break;
            case caesar: c = CAE(undo, c, mask[i]); break;
            default:     c = XOR(      c, mask[i]); break;
        }

        putchar(c);
        i = (i + 1) % len;
    }

    return EX_OK;
}

int main(int argc, char *argv[]) {
    int i;
    int opt;
    int masklen   = 0;
    int base      = 0;
    bool undo     = false;
    transform_t t = parse_tname(argv[0]);

    /* option parsing */
    while ((opt = getopt(argc, argv, "uxsvh")) != -1) {
        switch (opt) {
            case 'u':                        /* undo */
                if (t != xor) undo = true;
                break;
            case 'x':                        /* hex  */
                base = 16;
                break;
            case 's':                        /* string */
                base = -1;
                break;
            case 'v': return version(false, t);
            case 'h': return   usage(false, t);
            default:  return   usage(true,  t);
        }
    }

    if (optind >= argc) return usage(true, t);

    if (base != -1) {
        masklen = argc - optind;
        uint8_t mask[masklen];
        for (i = optind; i < argc; i++)
            mask[i-optind] = (uint8_t)strtol(argv[i], NULL, base);

        return action(t, mask, masklen, undo);
    } else {                                /* string */
        masklen = strlen(argv[optind]);
        uint8_t mask[masklen];
        for (i = 0; i < masklen; i++)
            mask[i] = (uint8_t)argv[optind][i];

        return action(t, mask, masklen, undo);
    }
}
