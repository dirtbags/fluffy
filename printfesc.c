#include <stdio.h>

int
main(int argc, char *argv[])
{
	printf("printf ");

	while (!feof(stdin)) {
		int c = getchar();

		switch (c) {
			case EOF:
				break;
			case '0' ... '9':
			case 'A' ... 'Z':
			case 'a' ... 'z':
				putchar(c);
				break;
			default:
				printf("\\\\%03o", c);
				break;
		}
	}

	putchar('\n');

	return 0;
}
