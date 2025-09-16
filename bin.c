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

		putchar(' ');
		for (int b = 7; b >= 0; b -= 1) {
			putchar(c>>b&1 ? '1' : '0');
		}
	}
	putchar('\n');

	return 0;
}
