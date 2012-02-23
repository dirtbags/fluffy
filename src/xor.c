/* xor filter -- 2012 Zephyr <zephyr@dirtbags.net>
 * 
 * This file is in the public domain.  I make no promises
 * about the functionality of this program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    int start = 1;
    int base = 0;
    int arg;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-x] m1 [m2 ...]\n", argv[0]);
        return 1;
    }

    if (0 == strcmp(argv[1], "-x")) {
        base = 16;
        start += 1;
    }

    arg = start;

    while (1) {
        int c = getchar();
        unsigned char mask;
        
        if (! argv[arg]) {
            arg = start;
        }
        mask = strtol(argv[arg++], NULL, base);

        if (EOF == c) {
            break;
        }

        c ^= mask;
        putchar(c);
    }

    return 0;
}
