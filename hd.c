#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "glyphs.h"

int dump(FILE *inf, bool verbose) {
  uint64_t p = 0;
  uint8_t buf[32];
  int offset = 0;
  int skipping = 0;

  while (!feof(inf)) {
    uint8_t *bytes = buf + offset;
    size_t len;
    int i;

    offset = 16 - offset;

    len = fread(bytes, 1, 16, inf);
    if (0 == len)
      break;

    if (!verbose && p && (len == 16) && (0 == memcmp(buf, buf + 16, 16))) {
      if (!skipping) {
        printf("⋮\n");
        skipping = 1;
      }
      p += 16;
      continue;
    } else {
      skipping = 0;
    }

    printf("%08lx  ", (long unsigned int)p);
    for (i = 0; i < 16; i += 1) {
      if (i < len) {
        printf("%02x ", bytes[i]);
      } else {
        printf("   ");
      }
      if (7 == i) {
        printf(" ");
      }
    }
    printf(" ");
    for (i = 0; i < len; i += 1) {
      printf("%s", fluffyglyphs[bytes[i]]);
    }
    if (-1 == printf("\n")) {
      perror("printf");
      return 1;
    }
    p += len;
  }
  printf("%08lx\n", (long unsigned int)p);

  return 0;
}

int main(int argc, char *argv[]) {
  FILE *f;
  bool verbose = false;
  int c;

  while ((c = getopt(argc, argv, "v")) != -1) {
    switch (c) {
      case -1:
        break;
      case 'v':
        verbose = true;
        break;
      default:
        fprintf(stderr, "Usage: %s [-v] [FILENAME]\n", argv[0]);
        fprintf(stderr, "\n");
        fprintf(stderr, "-v    Verbose: don't elide output if output lines are identical\n");
        return 1;
    }
  }

  if (!argv[optind] || (0 == strcmp("-", argv[optind]))) {
    f = stdin;
  } else {
    f = fopen(argv[optind], "rb");
    if (!f) {
      perror("open");
      return 1;
    }
  }

  dump(f, verbose);

  return 0;
}
