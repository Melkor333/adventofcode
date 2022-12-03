#include <stdio.h>
#define EOL '\n'
// A - 38 = 26, a - 96 = 1
#define ToPriority(num) ((num) - ((num) < 'a' ? 38 : 96))
int findChar(char *index, int len, char needle);
int findInSecondHalf(char *index, int len);

int main(int argc, char *argv[]) {
  char const *const filename = argv[1];
  char line[200];
  int c, count, sum;

  FILE *file = fopen(filename, "r");

  for (count = 0; ((c = fgetc(file)) != EOF) && count < 200; count++) {
    if (c != EOL)
      line[count] = c;
    else {
      c = findInSecondHalf(line, count);
      if (c > -1)
        sum += ToPriority(c);
      else {
        printf("YO Not found in second part!");
        return 1;
      }
      count = -1;
      c = *line;
    };
  };
  if (count == 200) {
    printf("Count went over 200 :(\n");
    return 1;
  }
  printf("%i\n", sum);
}

int findInSecondHalf(char *index, int len) {
  int i, c;
  int split = len/2;
  for (i=0; i < split; i++) {
    if ((c = findChar(index+split, split, index[i])) > -1) {
      printf("%i\n", c);
      return (int) index[c + split];
    };
  };
  return -1; // should never be reached
}

int findChar(char *index, int len, char needle) {
  int c;
  for (c = 0; c < len; c++) {
    if (index[c] == needle)
      return c;
  }
  return -1;
}
