#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const size_t MAXELVES = 500;
const size_t MAXCALLENGTH = 20;

int main(int argc, char *argv[]) {
  char const *const filename = argv[1];
  FILE *file = fopen(filename, "r");
  char *string[10];

  long int l;

  int curElve = 1;
  int strongestElve = 0;
  int secElve = 0;
  int thirdElve = 0;
  int curCal = 0;
  int maxCal = 0;
  int secCal = 0;
  int thirdCal = 0;

  // if (argv[1] == NULL)
  //   exit(1);

  if (file == NULL) {
    printf("FFF\n");
    exit(1);
  }

  while (fgets(string, sizeof(string), file) != NULL) {
    l = atoi(string);

    if (l == 0) {
      if (curCal > maxCal) {
        thirdElve = secElve;
        thirdCal = secCal;
        secElve = strongestElve;
        secCal = maxCal;
        strongestElve = curElve;
        maxCal = curCal;
      } else if (curCal > secCal) {
        thirdElve = secElve;
        thirdCal = secCal;
        secElve = curElve;
        secCal = curCal;
      } else if (curCal > thirdCal) {
        thirdElve = curElve;
        thirdCal = curCal;
      }
      curElve++;
      curCal = 0;
    } else {
      curCal += l;
    }
    // fp += l;
  }
  fclose(file);
  printf("The strongest Elves number %i %i %i carrying %li %li %li = %li Calories\n",
         strongestElve, secElve, thirdElve, maxCal, secCal, thirdCal, ((long) ((long) maxCal + (long) secCal) + (long) thirdCal));
}
