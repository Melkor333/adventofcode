#ifndef CRATES_H
#define CRATES_H

#include <stddef.h>


#define DEBUG

/* Max amount of Stacks */
#define MAX_CRATES 100
#define EOL '\n'

typedef struct stack_t{
  char crate;
  struct stack_t *next;
} stack_t;

typedef struct move_t {
  int amount;
  int from;
  int to;
} move_t;

char const *read_stack(char const *line, char *out, int n_stacks);
char const *get_stack(char const *lines, struct stack_t *stacks[], int n_stacks);
char const *get_shuffle_command(char const *lines, struct move_t *move, int n_stacks);
void shuffle_stacks(struct stack_t *stacks[] ,struct move_t move);
#endif
