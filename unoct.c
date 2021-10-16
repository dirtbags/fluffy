/*
 * Octal Decoder -- 2012 Zephyr <zephyr@dirtbags.net> This file is in the public domain.  I make no promises about the functionality 
 * of this program. 
 */

#include <stdio.h>

int
main(int argc, char *argv[])
{
	unsigned char acc = 0;
	unsigned char digit = 0;
	unsigned long int count = 0;

	while (1) {
		int c = getchar();

		count += 1;

		switch (c) {
			case EOF:
				return 0;
			case ' ':
			case '\r':
			case '\n':
			case '\t':
				digit += 3;
				break;
			case '0' ... '7':
				acc = (acc * 8) + c - '0';
				digit += 1;
				break;
			default:
				if (digit != 0) {
					fprintf(stderr, "Warning: non-numeric character mid-octet at offset %lu\n", count);
				}
				break;
		}

		if (digit > 3) {
			putchar(acc);
			acc = 0;
			digit = 0;
		}
	}

	return 0;
}
