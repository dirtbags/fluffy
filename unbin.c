#include <stdio.h>

int
main(int argc, char *argv[])
{
	unsigned char acc = 0;
	unsigned long int count = 0;
	unsigned char bits = 0;

	while (1) {
		count += 1;

		switch (getchar()) {
			case EOF:
				return 0;
			case '0':
				acc += 0;
				break;
			case '1':
				acc += 1;
				break;
			case ' ':
			case '\n':
				continue;
			default:
				fprintf(stderr, "Warning: non-binary character at offset %lu\n", count);
				break;
		}
		bits += 1;
		if (bits == 8) {
			putchar(acc);
			acc = 0;
			bits = 0;
		}
		acc <<= 1;
	}
	if (bits) {
		fprintf(stderr, "Warning: %d excess bits\n", bits);
	}

	return 0;
}
