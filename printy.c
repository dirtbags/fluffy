#include <stdio.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    int c;
    unsigned int count = 0;
    unsigned int printy = 0;

    for (;;) {
        c = getchar();
        if (EOF == c) {
            break;
        }
        if (isprint(c)) {
            printy += 1;
        }
        count += 1;
    }

    printf("%f\n", printy/(float)count);

    return 0;
}
