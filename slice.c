/*
 * slice up octet streams -- 2017 Neale Pickett <zephyr@dirtbags.net>
 *
 * This file is in the public domain.  I make no promises about the functionality
 * of this program. 
 */

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	int argn = 1;
	unsigned long int drop = true;
	unsigned long int next;
	unsigned long int pos;

	if (1 == argc) {
		fprintf(stderr, "Usage: %s start [end start...] [end]\n", argv[0]);
		fprintf(stderr, "\n");
		fprintf(stderr, "Slices input, keeping specified ranges\n");
		return 1;
	}

	next = strtoul(argv[argn], NULL, 0);

	for (pos = 0; ; pos += 1) {
		int c = getchar();

		if (EOF == c) {
			break;
		}

		if (next == pos) {
			drop = !drop;
		    argn += 1;
			if (argn == argc) {
				if (drop) {
					break;
				}
				next = ULONG_MAX;
			} else {
				next = strtoul(argv[argn], NULL, 0);
			}
		}

		if (drop) {
			/* drop */
		} else {
			putchar(c);
		}
	}

	return 0;
}
