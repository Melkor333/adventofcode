#include "du.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mydie(char *text) {
  puts(text);
  exit(1);
}

int main(int argc, const char *argv[]) {
  if (argc < 1)
    mydie("Not enough arguments");
  const char *filename = argv[1];
  char line[MAX_FILENAME+5]; // name + "$ cd "
  FILE *file = fopen(filename, "r");
  if (file == NULL)
    mydie("can't open file");

  if (fgets(line, MAX_FILENAME-1, file) == NULL)
    mydie("Error, couldn't read file");
  if (strncmp(line, "$ cd /\n", 7) != 0)
    mydie(strcat("First line is wrong: ", line));

  // Init pwd & add /
  PWD_INIT();
  mfile_t *root = create_mfile("/", 0);
  PWD_PUSH(root);
  d("cd to /");

  mfile_t *next;
  char name[MAX_FILENAME];
  int size = 0;

  // MAX_FILENAME - \0 + "$ cd "
  while (fgets(line, MAX_FILENAME-1+5, file) != NULL) {
    d(line);
    if (strncmp(line, "$ cd ..\n", 7) == 0) {
      d("cd ..");
      PWD_POP();
      printf("Dir: %s",PWD()->name);

    } else if ((sscanf(line, "$ cd %s", name)) == 1) {
      next = find_child(PWD(), name);
      if (next == NULL) {
          printf("%s has no child %s\n", PWD()->name, name);
          exit(1);
      }
      PWD_PUSH(next);

    } else if ((sscanf(line, "dir %s", name)) == 1) {
      printf("   Add child directory %s: %s\n", name, line);
      next = create_mfile(name, 0);
      add_child(PWD(), next);
    } else if (sscanf(line, "%d %s", &size, name) == 2) {
      printf("Add file %s with size %i: %s\n", name, size, line);
      next = create_mfile(name, size);
      add_child(PWD(), next);
    } else if (strcmp(line, "$ ls")) {
      d("Print children!");
      continue;
    } else {
      printf("Unknown line:\n%s\n", line);
      exit(1);
    }
  };

  get_size(root);
  //print_tree(root, 1);
  printf("Target: %i", 30000000-(70000000-root->size));
  get_matching_dir(root, 30000000-(70000000-root->size), 70000000);

  if (feof(file) == 0)
    d("Damn, not 'till eof!");
}
