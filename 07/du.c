#include <string.h>
#include "du.h"
#include <stdio.h>

mfile_t *find_child(mfile_t *file, char *name) {
  for (int i = 0; i < file->num_children; i++) {
    d(file->children[i]->name);

    if (file->children[i] == NULL) {
      printf("%s has no children", file->name);
      return NULL;
    }
    else if (strcmp((file->children[i])->name, name) == 0)
      return file->children[i];
    printf(" %s has child %s\n", file->name, file->children[i]->name);
  }
  return NULL;
}

mfile_t *create_mfile(char *name, int size){
  mfile_t *file = malloc(sizeof(mfile_t));
  if (file == NULL)
    return NULL;

  strcpy(file->name, name);
  file->children[0] = NULL;
  file->size = size;
  return file;
}

static int great_sum = 0;

void indent(int d) {
  for (int i = 0; i<d; i++)
    printf("  ");
}


void print_tree(mfile_t *file, int depth) {
  //indent(depth);
  if (file->num_children < 1)
    printf("%*c f %s\n", depth*2+8, '|' , file->name);
  else
    printf("%*c d %s\n", depth*2+8, '|', file->name);
  for (int i = 0; i < file->num_children; i++)
    print_tree(file->children[i], depth+1);
}

int get_size(mfile_t *file) {
  int size = 0;
  if (file->size > 0)
    return file->size;
  for (int i = 0; i < file->num_children; i++) {
    if (file->children[i] == NULL)
      break;
    size += get_size(file->children[i]);
  };
  file->size = size;
  if (size < 100000) {
    printf("%s is smaller than 100000: %i.", file->name, size);
    great_sum += size;
    printf(" Sum is now: %i\n", great_sum);
  } else {
    printf("  %s is greater than 100000: %i\n", file->name, size);
    printf("Sum is still: %i\n", great_sum);
  };

  return size;
}

int get_matching_dir(mfile_t *file, int target, int cur_size) {
  if (file->num_children < 1)
    return cur_size;
  if (file->size - target < 0)
    return cur_size; // our directory is too small!
  if (file->size < cur_size) {
    printf("new better directory: %s with %i", file->name, file->size);
    cur_size = file->size;
  }
  for (int i = 0; i < file->num_children; i++) {
    cur_size = get_matching_dir(file->children[i], target, cur_size);
  }

  return cur_size;
}

// if CD / initialize
// else -> warn & quit
// parse line
// if LS:
// if line->file Save name &size
// if line -> dir save name
//-->
// if CD ..
// pop PWD
// if CD NAME
// CD into [existing!]

// second loop
// for each directory
//  if file -> add to dirsum
//  if directory -> call self and add to dirsum
// return self.size

// third loop
// for each child directory
// if size < X -> add to sum

// fancy schmanzy:
// Print listing in dir-style fashion
