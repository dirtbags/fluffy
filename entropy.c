#include <stdio.h>
#include <math.h>

int
main(int argc, char *argv[])
{
	size_t len = 0;
	size_t hist[256] = {0};
	int c;
    int i;

	while ((c = getchar()) != EOF) {
		hist[c] += 1;
		len += 1;
	}

	float H = 0;
	for (i = 0; i < 256; i += 1) {
		if (hist[i]) {
			float Hi = (float)hist[i]/len;
			H -= Hi * log2(Hi);
		}
	}

	printf("%f\n", H);
}

