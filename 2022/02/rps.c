#include <stdio.h>
#include <stdlib.h>
//#include "rps.h"

#define WIN 6
#define DRAW 3
#define L 0
#define D 1
#define W 2
#define ROCK 0
#define PAPER 1
#define SCISSOR 2


int main(int argc, char *argv[]) {
  char const *const filename = argv[1];
  char string[4];
  FILE *file = fopen(filename, "r");
  int you, op;
  int all = 0;

  if (file == NULL) {
    printf("FFF\n");
    exit(1);
  }

  while (fgets(string, 5, file) != NULL) {
    if (string[0] < 'A' || string[0] > 'X')
      break;
    op = string[0] - 'A';
    you = string[2] - 'X';

    // Part 1
    //if (you == op) {
    //  all += DRAW;
    //} else if ((you == 0 && op == 2) || // rock beats scissors
    //           (you == 1 && op == 0 )|| // paper beats rock
    //           (you == 2 && op == 1))  // scissors beats paper
    //  all += WIN;
    //all += (you + 1);
   
    // Could probably be mathematically solved, idc :)
    //rock
    if ((op == ROCK && you == D) ||
        (op == PAPER && you == L) ||
        (op == SCISSOR && you == W))
      all += ROCK+1;
    //paper
    if ((op == ROCK && you == W) ||
        (op == PAPER && you == D) ||
        (op == SCISSOR && you == L))
      all += PAPER+1;
    //scissor
    if ((op == ROCK && you == L) ||
        (op == PAPER && you == W) ||
        (op == SCISSOR && you == D))
      all += SCISSOR+1;
    // Win/lose
    if (you == 1)
      all += 3;
    if (you == 2)
      all += 6;
    printf("you: %i, op: %i, all %i\n", you, op, all);
  }
  printf("%i", all);
}
