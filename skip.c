/*
 * skip octets -- 2017 Neale Pickett <zephyr@dirtbags.net>
 *
 * This file is in the public domain.  I make no promises about the functionality
 * of this program. 
 */

#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	long int count;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s count\n", argv[0]);
		return 1;
	}

	count = strtol(argv[1], NULL, 0);
	/* Throw away count octets */
	for (; count > 0; count -= 1) {
		int c = getchar();

		if (EOF == c) {
			break;
		}
	}

	/* Spit out the rest */
	while (1) {
		int c = getchar();

		if (EOF == c) {
			break;
		}
		putchar(c);
	}

	return 0;
}
