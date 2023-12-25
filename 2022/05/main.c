#include "crates.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (argc < 2)
    exit(1);
  char const *const filename = argv[1];
  char content[95000]; // Poor mans file->string
  int n_stacks = argv[2][0] - '0'; // 9 is the highest amount of stacks we take.

  stack_t *stacks[n_stacks];
  FILE *file = fopen(filename, "r");
  int temp;
  for (int i = 0; i < 95000; i++) {
    if ((temp = getc(file)) == EOF)
      break;
    content[i] = temp;
  }
  fclose(file);
  move_t next_move;
  char const *line;

  line = get_stack(content, stacks, n_stacks);
  //printf("%s", line);
  while(get_shuffle_command(line, &next_move, n_stacks) != NULL) {
    printf("move %i from %i to %i\n", next_move.amount, next_move.from, next_move.to);
    shuffle_stacks(stacks, next_move);
  }
  //for (int i = 0; i<n_stacks; i++)
  //  if (stacks[i] != NULL)
  //    if (stacks[i]->crate != '\0')
  //      printf("%c",stacks[i]->crate);
  printf("\n");
}
