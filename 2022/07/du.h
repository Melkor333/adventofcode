#ifndef du_h
#define du_h
#include <stdlib.h>
#include <stdio.h>

// Max children a directory can have
#define MAX_CHILDREN 1000

// how deep we can CD
#define MAX_NESTING 1000
#define MAX_FILENAME 1000

typedef struct mfile_t {
  struct mfile_t *children[MAX_CHILDREN];
  int num_children;
  int size;
  char name[MAX_FILENAME];
} mfile_t;

static struct _PWD {
  mfile_t *stack[MAX_NESTING];
  int pos;
} _PWD;

// TODO: Currently refining these Macros
#define d(text) puts(text)
#define PWD_INIT() _PWD.pos = -1; _PWD.stack[0] = NULL

// Who needs errors on overflows anyways!
#define PWD_POP() (_PWD.pos)--
#define PWD_PUSH(point) (_PWD.stack[++(_PWD.pos)] = (point))

#define PWD() (_PWD.stack[_PWD.pos])

mfile_t *create_mfile(char *name, int size);
#define add_child(parent, child) (parent)->children[((parent)->num_children)++] = (child)
mfile_t *find_child(mfile_t *file, char *name);
int get_size(mfile_t *file);
void print_tree(mfile_t *file, int depth);
int get_matching_dir(mfile_t *file, int target, int cur_size);
#endif
