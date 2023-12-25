#include "crates.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define NUM(num) ((num) - '0')

static char const *_LAST_INPUT = NULL;
static char const *_LAST_POS = NULL;

/* Doesn't check wheter it hit \n
   but will return NULL on empty or not [] surrounded value.*/
char const *read_stack(char const *line, char *out, int n_stacks) {
  if (line != _LAST_INPUT)
    _LAST_INPUT = line;
  else if (_LAST_POS != NULL)
    line = _LAST_POS;

  line++; // skip first possible [
  int found = 0;
  for (int c = 0; c < n_stacks; c++) {
    if (*line == ' ' )
      out[c] = '\0';
    else if (*(line -1) == '[' && *(line +1) == ']') {
      out[c] = *line;
      found = 1;
    }
    else
      return NULL;
    line += 4;
  };
  _LAST_POS = line;
  if (found == 1)
    return line - 1;
  else
    return NULL;
}

/* Returns new pointer position after reading stack */
char const *get_stack(char const *code, struct stack_t *stacks[], int n_stacks) {
  char crate_list[n_stacks];
  struct stack_t *bot_stack[n_stacks];
  for (int i = 0; i < n_stacks; i++) {
    stacks[i] = NULL;
    bot_stack[i] = NULL;
  }
  while ((code = read_stack(code, crate_list, n_stacks)) != NULL) {
    _LAST_POS = code;
    for (int i = 0; i < n_stacks; i++) {
      if (crate_list[i] != '\0') {
        struct stack_t *new_crate = (struct stack_t *) malloc(sizeof(struct stack_t)) ;
        new_crate->crate = crate_list[i];
        if (stacks[i] == NULL) {
          stacks[i] = new_crate;
        }
        else { // if stacks + i != NULL then bot_stack + i != NULL aswell
          bot_stack[i]->next = new_crate;
        }
        bot_stack[i] = new_crate;
      }
    }
  }
  return _LAST_POS;
}

/* Read a line and fill in the struct move_t. return pointer to new position in code */
char const *get_shuffle_command(char const *lines, struct move_t *move, int n_stacks) {
  static char const *_LAST_INPUT = NULL;
  static char const *_LAST_POS = NULL;
  if (lines != _LAST_INPUT)
    _LAST_INPUT = lines;
  else if (_LAST_POS != NULL)
    lines = _LAST_POS;
  lines--; // We wrote it as if we started at the \n
  //char const *c = lines + 6; later for next test
  if (lines[6] <= '0' || lines[6] >= '9')
      return NULL;
  int temp = 0; // NUM(lines[6]);
  int i = 0;
  while (lines[6 + i] >= '0' && lines[6 + i] <= '9') {
    temp = NUM(lines[6 + i]) + temp * pow(10, i);
    i++;
  }
  --i; // decrement last wrong increment
  if (lines[13+i] < '0' || lines[13+i] > (n_stacks + '0'))
      return NULL;
  if (lines[18+i] < '0' || lines[18+i] > (n_stacks + '0'))
      return NULL;

  move->amount = temp;
  move->from = NUM(lines[13 + i]);
  move->to = NUM(lines[18 + i]);
  _LAST_POS = lines + 20 + i;
  return lines + 20 + i;
}

void shuffle_stacks(struct stack_t *stacks[] ,struct move_t move) {
  struct stack_t *movingStack;
  for (int i = 1; i <= move.amount; i++) {
    movingStack = stacks[move.from-1];

    stacks[move.from-1] = movingStack->next;

    if (stacks[move.to-1] != NULL)
      movingStack->next = stacks[move.to-1];

    stacks[move.to-1] = movingStack;
    stacks[move.to-1] = movingStack;
  }
}
