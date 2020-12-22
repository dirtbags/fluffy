#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "glyphs.h"

int counts[256] = {0};

int main(int argc, char *argv[]) {
    int c;
    bool all = false;

    while ((c = getopt(argc, argv, "a")) != -1) {
        switch (c) {
            case -1:
                break;
            case 'a':
                all = true;
                break;
            default:
                fprintf(stderr, "Usage: %s [-a]\n", argv[0]);
                fprintf(stderr, "\n");
                fprintf(stderr, "-a    Output all octets, even if count == 0\n");
                return 1;
        }
    }

    for (;;) {
        c = getchar();
        if (EOF == c) {
            break;
        }
        counts[c] += 1;
    }

    for (c=0; c<256; ++c) {
        if (all || counts[c]) {
            printf("%d %02x %s\n", counts[c], c, fluffyglyphs[c]);
        }
    }
    return 0;
}