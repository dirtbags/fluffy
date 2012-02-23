/* Hex Decoder -- 2012 Zephyr <zephyr@dirtbags.net>
 * 
 * This file is in the public domain.  I make no promises
 * about the functionality of this program.
 */

#include <stdio.h>

int
main(int argc, char *argv[])
{
    unsigned char acc = 0;
    unsigned char nybble = 0;
    unsigned long int count = 0;

    while (1) {
        int c = getchar();

        count += 1;

        switch (c) {
            case EOF:
                return 0;
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
                if (nybble != 0) {
                    fprintf(stderr, "Warning: non-hex character mid-octet at offset %lu\n", count);
                }
                break;
        }

        if (nybble == 2) {
            putchar(acc);
            acc = 0;
            nybble = 0;
        }
    }

    return 0;
}
