#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_X 500
#define MAX_Y 500
#define MAX_VISIBLE_TREES (MAX_X*MAX_Y)

#define LINE_FINISHED 1
#define LEFT 2
#define RIGHT 4
#define DOWN 8
#define UP 16

typedef struct FOREST {
  int num_x;
  int num_y;
  int num_visible_trees;
  char *map[MAX_X];
  int visible_trees[MAX_VISIBLE_TREES];
} FOREST;

typedef struct TREE {
  int x;
  int y;
} TREE;

int tree_size(FOREST *forest, TREE *tree) {
  return forest->map[tree->x][tree->y] - '0';
}

void tree_copy(TREE *src, TREE *dst) {
  dst->x = src->x;
  dst->y = src->y;
}

int add_row(FOREST *forest, char *line) {
    forest->map[forest->num_x++] = strdup(line);
    if (forest->num_y == 0)
      forest->num_y = strlen(line);
    if (forest->num_x >= MAX_X) {
      puts("Too many rows!");
      exit(1);
    }
    return forest->num_x;
}

int get_next(FOREST *forest, TREE *tree, int mode) {
  if (mode == 0)
    mode = LEFT;
  else
    mode = mode & ~LINE_FINISHED;

  // go from left down
  if (mode & LEFT) {
    // init
    if (tree->x == -1) {
      tree->x = 0;
      tree->y = 0;
      return mode | LINE_FINISHED;
    }
    // go right
    if (++(tree->y) < forest->num_y) {
      return mode;
    // go one down and left
    } else if (++(tree->x) < forest->num_x) {
      tree->y = 0;
      return mode | LINE_FINISHED;
    }
    mode = RIGHT;
    tree->x = -1;
  }
  // go from right down
  if (mode & RIGHT) {
    // init
    if (tree->x == -1) {
      tree->x = 0;
      tree->y = forest->num_y - 1;
      return mode | LINE_FINISHED;
    }
    // go left
    if (--(tree->y) >= 0) {
      return mode;
    // go down and right
    } else if (++(tree->x) < forest->num_x) {
      tree->y = forest->num_y - 1;
      return mode | LINE_FINISHED;
    }
    mode = DOWN;
    tree->x = -1;
  }
  // go top right
  if (mode & DOWN) {
    //init
    if (tree->x == -1) {
      tree->x = 0;
      tree->y = 0;
      return mode | LINE_FINISHED;
    }
    // go one down
    if (++(tree->x) < forest->num_x) {
      return mode;
    // go one left and up to 0
    } else if (++(tree->y) < forest->num_y) {
      tree->x = 0;
      return mode | LINE_FINISHED;
    }
    mode = UP;
    tree->x = -1;
  }
  // go bot right
  if (mode & UP) {
    //init
    if (tree->x == -1) {
      tree->x = forest->num_x - 1;
      tree->y = 0;
      return mode | LINE_FINISHED;
    }
    // go one down
    if (--(tree->x) >= 0) {
      return mode;
    // go one right and fully down
    } else if (++(tree->y) < forest->num_y) {
      tree->x = forest->num_x - 1;
      return mode | LINE_FINISHED;
    }
    tree->x = -1;
    return -1;
  }
  puts("something went wrong. no mode applies");
  exit(1);
}

// add a visible tree to a forest
int find_visible_tree(FOREST *forest, TREE *tree, int add) {
  // merge x and y into one number: X0Y the x=1 is so that x may be 0
  int coord = (tree->x+1)*1000 + tree->y;
  for (int i = 0; i < forest->num_visible_trees; i++) {
    if (coord == forest->visible_trees[i]) {
      return 0;
    }
  }
  if (add == 1)
    forest->visible_trees[(forest->num_visible_trees)++] = coord;
  if (forest->num_visible_trees >= MAX_VISIBLE_TREES) {
    printf("Too many visible trees!");
    exit(1);
  }
  return 1;
}

void find_visible_trees(FOREST *forest) {
  TREE *tallest_tree;
  TREE *tree;
  int i;
  tree->x = -1;
  tree->y = -1;

  int max = 0;
  int mode = 0;
  while ((mode = get_next(forest, tree, mode)) > 0) {
    if (mode & LINE_FINISHED || tree_size(forest, tree) > max) {
      find_visible_tree(forest, tree, 1); // almost forgot, EVERY VISIBLE TREE. Not just the tallest one!
      max = tree_size(forest, tree);
    }
  }
}

int main(int argc, const char *argv[]) {
  if (argc < 2)
    exit(1);
  const char *filename = argv[1];
  FILE *file = fopen(filename, "r");
  FOREST forest;
  forest.num_x = 0;
  forest.num_y = 0;
  forest.num_visible_trees = 0;
  char line[MAX_Y];
  int col = 0;

  while (fgets(line, MAX_Y-1, file) != NULL) {
    add_row(&forest, line);
  }

  for (int i = 0; i<forest.num_visible_trees-1; i++)
    printf("%i\n", forest.visible_trees[i]);

  find_visible_trees(&forest);

  // create forest map
  int secondcount = 0;
  for (int x = 0; x < forest.num_x; x++) {
    for (int y = 0; y < forest.num_y -1; y++) {
      TREE t;
      t.x = x;
      t.y = y;
      if(find_visible_tree(&forest, &t, 0) == 0) {
        printf("\033[0;31m%c\033[0m", forest.map[x][y]);
        secondcount++;
      } else
        printf("%c", forest.map[x][y]);
    }
    printf("\n");
  }
  printf("Number of trees is: %i\n", forest.num_visible_trees);
  printf("Map has size: %i %i\n", forest.num_x, forest.num_y);
  printf("Second count is: %i\n", secondcount);
}
