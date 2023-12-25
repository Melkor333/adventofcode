#include "startofmarker.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *const argv[]) {
  if (argc < 1)
    exit(1);
  char *filename = argv[1];
  int start_size = atoi(argv[2]);
  FILE *file = fopen(filename, "r");

  printf("%i",get_package_start(file, start_size));
}
