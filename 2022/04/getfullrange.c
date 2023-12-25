#include <stdio.h>

int main(int argc, char *argv[]) {
  char const *const filename = argv[1];
  char c, number[5];
  int sum, currentNumber = 0;
  int numbers[] = { 0, 0, 0, 0 };

  FILE *file = fopen(filename, "r");


  while ((c = fgetc(file)) != EOF) {
    printf("%c", c);
    if (c >= '0' && c <= '9') {
      numbers[currentNumber] = (numbers[currentNumber]*10) + (c - '0'); // poor atoi
    }
    else if (c == ',' || c == '-')
      currentNumber++;
    else {
      printf("\n");
      //day1
      //if ((numbers[0] <= numbers[2] && numbers[1] >= numbers[3]) ||
      //    (numbers[2] <= numbers[0] && numbers[3] >= numbers[1])) {
      if ((numbers[0] <= numbers[2] && numbers[1] >= numbers[3]) ||
          (numbers[2] <= numbers[0] && numbers[3] >= numbers[1]) || // complete overlap until here
          (numbers[1] >= numbers[3] && numbers[0] <= numbers[3]) || // partial overlap
          (numbers[0] <= numbers[2] && numbers[1] >= numbers[2])) {
        sum++;
        printf("overlap!\n");
      }
      currentNumber = 0;
      numbers[0]= numbers[1]= numbers[2]= numbers[3] = 0;
    }
  }
  printf("%i\n", sum);
}
