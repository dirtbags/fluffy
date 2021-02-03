/*
 * xor filter -- 2020 Neale Pickett <neale@woozle.org>
 *
 * This file is in the public domain.  I make no promises about the functionality
 * of this program. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  int radix = 0;
  int arg;
  int c;

  while ((c = getopt(argc, argv, "x")) != -1) {
    switch (c) {
      case 'x':
        radix = 16;
        break;
      default:
        fprintf(stderr, "Usage: %s [-x] m1 [m2 ...]\n", argv[0]);
        return 1;
    }
  }

  if (!argv[optind]) {
    return 1;
  }

  arg = optind;

  while (1) {
    int c = getchar();
    unsigned char mask;

    if (!argv[arg]) {
      arg = optind;
    }
    mask = strtol(argv[arg++], NULL, radix);

    if (EOF == c) {
      break;
    }

    c ^= mask;
    putchar(c);
  }

  return 0;
}
