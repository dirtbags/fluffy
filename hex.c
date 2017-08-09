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
		printf("%02x ", c);
		if (7 == count % 8) {
			putchar(' ');
		}
	}
	putchar('\n');

	return 0;
}
