/*
 * drop octets -- 2017 Neale Pickett <zephyr@dirtbags.net>
 *
 * This file is in the public domain.  I make no promises about the functionality
 * of this program. 
 */

#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	unsigned long int start;
	unsigned long int end;
	unsigned long int pos;

	if (3 == argc) {
		start = strtoul(argv[1], NULL, 0);
		end = strtoul(argv[2], NULL, 0);
	} else {
		fprintf(stderr, "Usage: %s start end\n", argv[0]);
		fprintf(stderr, "\n");
		fprintf(stderr, "Drops octets from input\n");
		return 1;
	}

	for (pos = 0; ; pos += 1) {
		int c = getchar();

		if (EOF == c) {
			break;
		}

		if ((start <= pos) && (pos < end)) {
			/* drop */
		} else {
			putchar(c);
		}
	}

	return 0;
}
