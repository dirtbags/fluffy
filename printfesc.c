#include <stdio.h>
#include <ctype.h>

int
main(int argc, char *argv[])
{
	printf("printf ");

	while (!feof(stdin)) {
		int c = getchar();

		switch (c) {
			case EOF:
				break;
			case '\n':
				printf("\\n");
				break;
			case '\r':
				printf("\\r");
				break;
			case '\t':
				printf("\\t");
				break;
			case '"':
				printf("\\\"");
				break;
			default:
				if (isprint(c)) {
					putchar(c);
				} else {
					printf("\\%03o", c);
				}
				break;
		}
	}

	putchar('\n');

	return 0;
}
