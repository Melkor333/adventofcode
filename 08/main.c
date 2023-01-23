#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_ROW 500
#define MAX_COL 500
#define MAX_VISIBLE_TREES (MAX_ROW+MAX_COL)*2

int find_tree(int trees[], int num, int len) {
  for (int i = 0; i<len; i++)
    if (num == trees[i])
      return 1;
  return 0;
}

int main(int argc, const char *argv[]) {
  if (argc < 2)
    exit(1);
  const char *filename = argv[1];
  FILE *file = fopen(filename, "r");

  char *tree_map[MAX_ROW];
  char line[MAX_COL];
  int num_rows = 0;
  int num_col = 0;
  int col = 0;
  int visible_trees[MAX_VISIBLE_TREES];
  int num_visible_trees = 0;

  while (fgets(line, MAX_COL-1, file) != NULL) {
    if (num_rows >= MAX_ROW) {
      printf("fuckyducki too many Rows!");
      exit(1);
    }
    tree_map[num_rows] = strdup(line);
    num_rows++;
  }
  // TODO: Make this a function which takes a function to calculate next x/y!
  // See from the left
  for (int x; x < num_rows; x++) {
    int max_y = 0;
    int max = 0;
    int tree = 0;
    for (int y = 0; y < MAX_COL; y++) {
      if (tree_map[x][y] == '\n' || tree_map[x][y] == '\0')
        break;
      int cur_tree = tree_map[x][y] - '0';
      if (cur_tree > max) {
        max = cur_tree;
        max_y = y;
      }
    }
    if (!(find_tree(visible_trees, (x+1)*1000 + max_y, num_visible_trees))) {
      visible_trees[num_visible_trees++] = (x+1)*1000 + max_y;
    }
  }

  puts("Visible Trees from left:");
  for (int i = 0; i<num_visible_trees; i++)
    printf("%i\n", visible_trees[i]);

  printf("Number of trees is: %i", num_visible_trees);
}
