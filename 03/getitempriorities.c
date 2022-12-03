#include <stdio.h>
#define EOL '\n'

int main(int argc, char *argv[]) {
  char const *const filename = argv[1];
  char line[200];
  char *bag, *bag2, *count2;
  int c, count, sum;

  printf("%i %i\n", (int) 'a', (int)'A');
  FILE *file = fopen(filename, "r");

  for (count = 0; ((c = fgetc(file)) != EOF) && count < 200; count++) {
    if (c != EOL)
      line[count] = c;
    else {
      bag = line;
      bag2 = line + (count / 2);

      for (; bag < bag2; bag++) {
        count2 = bag2;
        for (count2 = bag2;
             (*count2 != *bag) && (count2 - line < (count-1));
             count2++)
          ;
        if (*count2 == *bag) {
          sum += *bag - (*bag < 'a' ? 38 : 96); // A - 39 = 26, a - 96 = 1
          printf("%i %i %c\n", sum, (*bag < 'a' ? 38 : 96), *bag);
          printf("%i %li %li\n\n", count, count2 - line, bag2 - line);
          break;
        }
      }
      count = -1;
    }
  };
  if (count == 200) {
    printf("Count went over 200 :(\n");
    return 1;
  }
  printf("%i\n", sum);
}
