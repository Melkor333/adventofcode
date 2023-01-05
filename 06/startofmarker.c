#include "startofmarker.h"
#include <stdio.h>
#include <stdlib.h>
#define MAX_BITS 20

int count_bits(uint32_t bits[], int l) {
  uint32_t sum = 0;
  for (int i = 0; i<l; i++) {
    sum |= bits[i];
  }

  printf("currently %i different bits\n", __builtin_popcount(sum));
  return __builtin_popcount(sum);
}

int get_package_start(FILE *text, int size) {
  if (size > MAX_BITS) {
    printf("Too big size");
    exit(1);
  }
  int i, count, l;
  count = 0;
  l = -1;

  uint32_t bits[MAX_BITS];
  for (int i = 0; i < size; i++)
    bits[i] = 0;

  while ((i = getc(text)) != EOF) {
    count++;
    if (l < (size-1))
      l++;
    else
      l = 0;

    bits[l] = 1 << (i % 32);

    // start after reading 4
    if (count < size) {
      continue;
    } else if (count_bits(bits, size) == size)
      return count;
  }
  return -1;
}
