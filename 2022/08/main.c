#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_X 500
#define MAX_Y 500
#define MAX_VISIBLE_TREES (MAX_X*MAX_Y)
#define COORD(f,x,y) (x)*((f)->num_y)+(y)

#define LINE_FINISHED 1
#define LEFT 2
#define RIGHT 4
#define DOWN 8
#define UP 16
#define OVERRIDE 32
#define HOUSE 64

typedef struct FOREST {
  int num_x;
  int num_y;
  int num_visible_trees;
  char *map[MAX_X];
  int visible_trees[MAX_VISIBLE_TREES];
  int house_visible_trees[MAX_VISIBLE_TREES];
  int tree_scenic[MAX_VISIBLE_TREES];
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
    if (forest->num_y == 0)
      forest->num_y = strlen(line);
    forest->map[forest->num_x++] = strdup(line);
    if (forest->num_x >= MAX_X) {
      puts("Too many rows!");
      exit(1);
    }
    return forest->num_x;
}

int next_tree(FOREST *forest, TREE *tree, long mode) {
  //printf("%i", mode);
  if (mode == 0) {
    mode = LEFT | OVERRIDE; // Override on first run
    tree->x = -1;
  } else
    mode = mode & (~LINE_FINISHED);
  //printf("get next after %i %i\n", tree->x, tree->y);

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
void set_tree_visibility(FOREST *forest, TREE *tree, int visible, long mode) {
  // merge x and y into one number: X0Y the x=1 is so that x may be 0
  //int coord = (tree->x+1)*1000 + tree->y;
  int coord = COORD(forest,tree->x,tree->y);
  //printf("%i * %i + %i = %i\n", tree->x,forest->num_y,tree->y, coord);
  forest->house_visible_trees[coord] = 0;
  if (mode & OVERRIDE)
    forest->visible_trees[coord] = visible;
  else
    forest->visible_trees[coord] |= visible;
}

int tree_visibility(FOREST *forest, TREE *tree, long mode) {
  // merge x and y into one number: X0Y the x=1 is so that x may be 0
  //int coord = (tree->x+1)*1000 + tree->y;
  int coord = COORD(forest,tree->x,tree->y);
  if (mode & HOUSE)
    return forest->house_visible_trees[coord];
  else
    return forest->visible_trees[coord];
}

void find_visible_trees(FOREST *forest) {
  TREE *tallest_tree;
  TREE *tree;
  tree->x = -1;
  tree->y = -1;

  int max = -1;
  long mode = 0;
  while ((mode = next_tree(forest, tree, mode)) > 0) {
    if (mode & LINE_FINISHED || tree_size(forest, tree) > max) {
      set_tree_visibility(forest, tree, 1, mode);
      max = tree_size(forest, tree);
    } else
      set_tree_visibility(forest, tree, 0, mode);
  }
}

int house_find_visible_trees(FOREST *forest, TREE *tree, int add_to_map) {
  TREE tree_house;
  int distance = 0;
  int full_dist = 1;
  tree_house.x = tree->x;
  tree_house.y = tree->y;

  int size = 0;
  long mode = LEFT;
  long last_mode = LEFT;
  int looped = 0;
  while ((mode = next_tree(forest, tree, mode)) > 0) {
    if (((mode & LINE_FINISHED) == 0) && tree_size(forest, tree) >= size) {
      size = tree_size(forest, tree);
      distance++;
      if (add_to_map == 1)
        forest->house_visible_trees[COORD(forest,tree->x,tree->y)] = 1;
      continue;
    }
    tree->x = tree_house.x;
    tree->y = tree_house.y;
    if (add_to_map == 1)
      printf("calc %i * %i", distance, full_dist);
    if (distance > 0)
      full_dist *= distance;
    distance = 0;
    size = 0;
    if (mode & last_mode) {
      if (looped == 1)
        goto finished;
      if (mode & LEFT)
        last_mode = mode = RIGHT;
      else if (mode & RIGHT)
        last_mode = mode = DOWN;
      else if (mode & DOWN) {
        last_mode = mode = UP;
        looped = 1;
      } else if (mode & UP)
        goto finished;
    } else
      last_mode = mode & (LEFT | RIGHT | UP | DOWN);
  }
 finished:
  return full_dist;
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

  //for (int i = 0; i<forest.num_visible_trees-1; i++)
  //  printf("%i\n", forest.visible_trees[i]);

  find_visible_trees(&forest);

  // create forest map
  int secondcount = 0;
  int houses_visible_trees[MAX_VISIBLE_TREES];
  TREE tree;
  for (tree.x = 0; tree.x < forest.num_x; (tree.x)++) {
    for (tree.y = 0; tree.y < forest.num_y -1; (tree.y)++) {
      if(tree_visibility(&forest, &tree, 0))
        secondcount++;
      int coord = COORD(&forest,tree.x,tree.y);
      forest.house_visible_trees[COORD(&forest,tree.x,tree.y)] = 0;
      houses_visible_trees[coord] = house_find_visible_trees(&forest, &tree, 0);
    }
  }
  int max_scenic = 0;
  TREE best_house;
  for (int x = 0; x < forest.num_x; x++) {
    for (int y = 0; y < forest.num_y -1; y++) {
      int coord = x*forest.num_y+y;
      if (max_scenic < houses_visible_trees[coord]) {
        max_scenic = houses_visible_trees[coord];
        best_house.x = x;
        best_house.y = y;
      }
    }
  }
  house_find_visible_trees(&forest, &best_house, 1);
  for (int x = 0; x < forest.num_x; x++) {
    for (int y = 0; y < forest.num_y -1; y++) {
      TREE t;
      t.x = x;
      t.y = y;
      if (forest.house_visible_trees[COORD(&forest,x,y)] == 1)
        printf("\033[0;93m%c\033[0m", forest.map[x][y]);
      else if(tree_visibility(&forest, &t, 0)) {
        printf("\033[0;31m%c\033[0m", forest.map[x][y]);
      } else
        printf("%c", forest.map[x][y]);
        //printf("%i", tree_visibility(&forest, &t));
    }
    printf("\n");
  }

  printf("Number of trees is: %i\n", forest.num_visible_trees);
  printf("Map has size: %i %i\n", forest.num_x, forest.num_y);
  forest.num_visible_trees = secondcount;
  printf("Second count is: %i\n", secondcount);
  printf("Best house coord is %i,%i with a scenic count of %i\n", best_house.x, best_house.y, max_scenic);
}
