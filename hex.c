#include <stdio.h>

int
main(int argc, char *argv[])
{
	size_t count;

	for (count = 0; ; count += 1) {
		int c = getchar();

		if (EOF == c) {
			break;
		}

		if (count) {
			putchar(' ');
			if (0 == count % 8) {
				putchar(' ');
			}
		}

		printf("%02x", c);
	}
	putchar('\n');

	return 0;
}
