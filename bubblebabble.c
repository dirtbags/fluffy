#include <stdio.h>

/** Compute bubble babble for input buffer.
 *
 * The generated output will be of length 6*((inlen/2)+1), including the
 * trailing NULL.
 *
 * Test vectors:
 *     `' (empty string) `xexax'
 *     `1234567890'      `xesef-disof-gytuf-katof-movif-baxux'
 *     `Pineapple'       `xigak-nyryk-humil-bosek-sonax'
 */
static char const consonants[] = "bcdfghklmnprstvz";
static char const vowels[]     = "aeiouy";

int
main(int argc, char *argv[])
{
  int seed = 1;

  putchar('x');
  while (1) {
    int c;

    c = getchar();
    if (EOF == c) {
      putchar(vowels[seed % 6]);
      putchar('x');
      putchar(vowels[seed / 6]);
      break;
    }

    putchar(vowels[(((c >> 6) & 3) + seed) % 6]);
    putchar(consonants[(c >> 2) & 15]);
    putchar(vowels[((c & 3) + (seed / 6)) % 6]);

    seed = (seed * 5) + (c * 7);
    c = getchar();
    seed = (seed + c) % 36;

    if (EOF == c) {
      break;
    }
    putchar(consonants[(c >> 4) & 15]);
    putchar('-');
    putchar(consonants[c & 15]);
  }

  putchar('x');
  putchar('\n');

  return 0;
}
