#include <stdio.h>
#define EOL '\n'
// A - 38 = 26, a - 96 = 1
#define ToPriority(num) ((num) - ((num) < 'a' ? 38 : 96))
struct line {
  char string[200];
  int len;
};
int findChar(char *index, int len, char needle);
int findInSecondHalf(char *index, int len);
int ChallengeOne(char *line, int count, int *sum);
int ChallengeTwo(struct line group[]);

int main(int argc, char *argv[]) {
  char const *const filename = argv[1];
  struct line group[3];
  int c, count, sum, groupC;
  groupC = sum = 0;

  FILE *file = fopen(filename, "r");

  for (count = 0; ((c = fgetc(file)) != EOF) && count < 199; count++) {
    if (c != EOL)
      group[groupC].string[count] = c;
    else {
      // ChallengeOne(line, count, &sum);
      // count = -1;

      group[groupC].len = count;
      group[groupC].string[count+1] = '\0';
      if (groupC < 2)
        groupC++;
      else {
        sum += ChallengeTwo(group);
        groupC = 0;
      }
      count = -1;
    };
  };
  if (count == 200) {
    printf("Count went over 200 :(\n");
    return 1;
  }
  printf("%i\n", sum);
}

int ChallengeTwo(struct line group[]) {
  int count, groupC, foundC, c;
  char needle;
  printf("%s, %i\n", group[0].string, group[0].len);
  for (count = 0; count < group[0].len; count++) {
    foundC = 0;
    needle = group[0].string[count];
    printf("%c ", needle);
    for (groupC = 1; groupC < 3; groupC++) {
      printf("%s, %i\n", group[groupC].string, group[groupC].len);
      if ((c = findChar(group[groupC].string, group[groupC].len, needle)) > -1) {
        printf("found %c in %s\n", needle, group[groupC].string);
        if (foundC == 1)
          return ToPriority(needle);
        else
          foundC++;
      }
    }
  }
  printf("fugg: %c\n", c);
  return -1;
}

int ChallengeOne(char *line, int count, int *sum) {
  char c;
  c = findInSecondHalf(line, count);
  if (c > -1) {
    *sum += ToPriority(c);
    return 0;
  }
  else {
    printf("YO Not found in second part!");
    return -1;
  }
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
