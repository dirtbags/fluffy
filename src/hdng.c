/*
 * Last Modified: Fri 17 Feb 2012 12:08:33 PM CST
 *
 *   _   _Hex Dumper _      ____                           _   _
 *  | \ | | _____  _| |_   / ___| ___ _ __   ___ _ __ __ _| |_(_) ___  _ __
 *  |  \| |/ _ \ \/ / __| | |  _ / _ \ '_ \ / _ \ '__/ _` | __| |/ _ \| '_ \
 *  | |\  |  __/>  <| |_  | |_| |  __/ | | |  __/ | | (_| | |_| | (_) | | | |
 *  |_| \_|\___/_/\_\\__|  \____|\___|_| |_|\___|_|  \__,_|\__|_|\___/|_| |_|
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sysexits.h>
#include <string.h>
#include <getopt.h>
#include "netre.h"

const char* charset[] = {
    "·", "☺", "☻", "♥", "♦", "♣", "♠", "•", "◘", "○", "◙", "♂", "♀", "♪", "♫", "☼",
    "►", "◄", "↕", "‼", "¶", "§", "▬", "↨", "↑", "↓", "→", "←", "∟", "↔", "▲", "▼",
    " ", "!", "\"", "#", "$", "%", "&", "'", "(", ")", "*", "+", ",", "-", ".", "/",
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ":", ";", "<", "=", ">", "?",
    "@", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
    "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "[", "\\", "]", "^", "_",
    "`", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o",
    "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "{", "|", "}", "~", "⌂",
    "Ç", "ü", "é", "â", "ä", "à", "å", "ç", "ê", "ë", "è", "ï", "î", "ì", "Ä", "Å",
    "É", "æ", "Æ", "ô", "ö", "ò", "û", "ù", "ÿ", "Ö", "Ü", "¢", "£", "¥", "€", "ƒ",
    "á", "í", "ó", "ú", "ñ", "Ñ", "ª", "º", "½", "⅓", "¼", "⅕", "⅙", "⅛", "«", "»",
    "░", "▒", "▓", "│", "┤", "╡", "╢", "╖", "╕", "╣", "║", "╗", "╝", "╜", "╛", "┐",
    "└", "┴", "┬", "├", "─", "┼", "╞", "╟", "╚", "╔", "╩", "╦", "╠", "═", "╬", "╧",
    "╨", "╤", "╥", "╙", "╘", "╒", "╓", "╫", "╪", "┘", "┌", "█", "▄", "▌", "▐", "▀",
    "α", "ß", "Γ", "π", "Σ", "σ", "µ", "τ", "Φ", "Θ", "Ω", "δ", "∞", "φ", "ε", "∩",
    "⁰", "¹", "²", "³", "⁴", "⁵", "⁶", "⁷", "⁸", "⁹", "ⁱ", "ⁿ", "⁽", "⁼", "⁾", "¤"
};

int version(bool error) {
    fprintf(WHICHOUT(error), "hdng v.%s - %s\n\n", PACKAGE_VERSION,
            "The Next Generation Hex Dumper");
    return error ? EX_USAGE : EX_OK;
}

int usage(bool error, char *prog) {
    int retval = version(error);
    fprintf(WHICHOUT(error), "Usage: %s [ [-oxsg] [-w width] | -X | -G ] [filename]\n", prog);
    fprintf(WHICHOUT(error), "\t-o\tdo not print offsets\n");
    fprintf(WHICHOUT(error), "\t-x\tdo not print hexadecimal bytes\n");
    fprintf(WHICHOUT(error), "\t-s\tdo not abbreviate redundant bytes\n");
    fprintf(WHICHOUT(error), "\t-g\tdo not print glyphs\n");
    fprintf(WHICHOUT(error), "\t-w\tlength of data to show on each line\n\n");
    fprintf(WHICHOUT(error), "\t-X\thex dump mode (continuous)\n");
    fprintf(WHICHOUT(error), "\t-G\tglyph dump mode (continuous)\n\n");
    fprintf(WHICHOUT(error), "\tif a filename is not specified, stdin will be used\n");
    return retval;
}

void print_hexits(uint8_t buf[], size_t length, bool continuous) {
    size_t i;

    for (i = 0; i < length; i += 1) {
        printf("%02x ", buf[i]);
        if (continuous && ((i+1) % 8 == 0)) printf(" ");
    }
    if (continuous) printf(" ");
}

void print_glyphs(uint8_t buf[], size_t length, bool decoration) {
    size_t i;

    if (decoration) printf("┆");

    for (i = 0; i < length; i += 1)
        printf("%s", charset[buf[i]]);

    if (decoration) printf("┆");
}

int dump(FILE *f, uint8_t width, bool offsets, bool hex, bool glyphs, bool skip, bool nl) {
    uint64_t p        = 0;
    uint64_t op       = 0;
    int      skipping = 0;
    size_t   offset   = 0;
    uint8_t  buf[2*width];

    while (!feof(f)) {
        uint8_t *bytes = buf + offset;
        size_t  len;

        offset = width - offset;

        len = fread(bytes, 1, width, f);
        if (len == 0) break;
        op = p;
        p += len;

        if (skip) {
            if (op && (memcmp(buf, buf + width, width) == 0)) {
                if (skipping == 1) {
                    printf("*\n");
                    skipping = 1;
                }
                continue;
            } else {
                skipping = 0;
            }
        }

        if (offsets) printf("%08lx  ", (long unsigned int)op);
        if (hex)     print_hexits(bytes, len, nl);
        if (glyphs)  print_glyphs(bytes, len, hex);
        if (nl)      printf("\n");
    }

    if (offsets)     printf("%08lx bytes", (long unsigned int)p);
    printf("\n");
    return EX_OK;
}

int main(int argc, char *argv[]) {
    int opt;
    FILE *input = stdin;

    /* defaults */
    uint8_t width   = 16;
    bool    hex     = true;
    bool    skip    = true;
    bool    glyphs  = true;
    bool    offsets = true;
    bool    nl      = true;

    /* special modes */
    bool hexdump   = false;
    bool glyphdump = false;

    /* option parsing */
    while ((opt = getopt(argc, argv, "hvoxsgw:XG")) != -1) {
        switch (opt) {
            case 'o': /* offset off */
                offsets = false;
                break;
            case 'x': /* hex off */
                hex = false;
                break;
            case 's': /* skip off */
                skip = false;
                break;
            case 'g': /* glyphs off */
                glyphs = false;
                break;
            case 'w': /* width */
                if (optarg[0] == '-') optarg++;
                width = (uint8_t)atoi(optarg);
                if (width == 0) width = 16;
                break;
            case 'X': /* hex dump mode */
                hexdump = true;
                break;
            case 'G': /* glyph dump mode */
                glyphdump = true;
                break;
            case 'v': return version(false);
            case 'h': return usage(false, argv[0]);
            default:  return usage(true,  argv[0]);
        }
    }

    /* crazy user! */
    if (!hex && !glyphs) glyphs = true;

    /* modes override other options, hex wins vs. glyphs */
    if (hexdump) {
        glyphs = skip = offsets = nl = false;
        hex    = true;
        width  = 16;
    } else if (glyphdump) {
        hex    = skip = offsets = nl = false;
        glyphs = true;
        width  = 16;
    }

    if (optind < argc) {
        input = fopen(argv[optind], "rb");
        if (!input) {
            perror("open");
            return EX_OSFILE;
        }
    }

    return dump(input, width, offsets, hex, glyphs, skip, nl);
}
