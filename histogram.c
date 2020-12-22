#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  int lineno = 0;
  int divisor = 1;
  int c;

  while ((c = getopt(argc, argv, "d:")) != -1) {
    switch (c) {
      case 'd':
        divisor = (atoi(optarg));
        if (divisor > 0) {
          break;
        }
        // fallthrough
      default:
        fprintf(stderr, "Usage: %s [-s] [-d DIVISOR]\n", argv[0]);
        fprintf(stderr, "\n");
        fprintf(stderr, "-d DIVISOR   Divide bar width by DIVISOR\n");
        return 1;
    }
  }

  for (;;) {
    char line[128];
    int count;
    int ret;

    ++lineno;
    ret = scanf("%d %127[^\n]\n", &count, line);
    if (EOF == ret) {
      break;
    } else if (ret < 2) {
      fprintf(stderr, "Unparseable input on line %d\n", lineno);
      scanf("%*[^\n]\n");  // Read in and discard one line
      continue;
    }
    printf("%s ", line);
    for (int i = 0; i < count / divisor; ++i) {
      putchar('#');
    }
    printf(" %d\n", count);
  }
  return 0;
}
