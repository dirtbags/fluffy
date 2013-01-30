#include <stdio.h>

int
main(int argc, char *argv[])
{
	while (1) {
		int c = getchar();

		switch (c) {
			case EOF:
				return 0;
			case 134:
				printf("\\\\");
				break;
			case 32 ... 91:
			case 93 ... 126:
				putchar(c);
				break;
			default:
				printf("\\x%02x", c);
				break;
		}
	}

	return 0;
}
