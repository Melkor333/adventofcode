#include <string.h>
#include "../test-framework/unity.h"
#include "crates.h"


void setUp(void)
{
}

void tearDown(void)
{
}

static void test_only_first(void)
{
  char crate_list[3];
  read_stack("[Z]        ", crate_list, 3);
  TEST_ASSERT_EQUAL('Z', crate_list[0]);
  TEST_ASSERT_EQUAL('\0', crate_list[1]);
  TEST_ASSERT_EQUAL('\0', crate_list[2]);
}

static void test_only_last(void)
{
  char crate_list[3];
  read_stack("        [Z]", crate_list, 3);
  TEST_ASSERT_EQUAL('\0', crate_list[0]);
  TEST_ASSERT_EQUAL('\0', crate_list[1]);
  TEST_ASSERT_EQUAL('Z', crate_list[2]);
}

static void test_middle(void)
{
  char crate_list[3];
  read_stack("    [Z]    ", crate_list, 3);
  TEST_ASSERT_EQUAL('\0', crate_list[0]);
  TEST_ASSERT_EQUAL('Z', crate_list[1]);
  TEST_ASSERT_EQUAL('\0', crate_list[2]);
}

static void test_all_but_middle(void)
{
  char crate_list[3];
  read_stack("[Z]     [Z]", crate_list, 3);
  TEST_ASSERT_EQUAL('Z', crate_list[0]);
  TEST_ASSERT_EQUAL('\0', crate_list[1]);
  TEST_ASSERT_EQUAL('Z', crate_list[2]);
}

static void test_empty(void)
{
  char crate_list[3];
  TEST_ASSERT_EQUAL(NULL, read_stack("           ", crate_list, 3));
  TEST_ASSERT_EQUAL('\0', crate_list[0]);
  TEST_ASSERT_EQUAL('\0', crate_list[1]);
  TEST_ASSERT_EQUAL('\0', crate_list[2]);
}

static void test_invalid(void)
{
  char crate_list[3];
  TEST_ASSERT_EQUAL(NULL , read_stack(" 1   2   3 ", crate_list, 3));
}

static void test_pointer_return(void)
{
  char crate_list[3];
  char const string[] = "[Z]     [Z]\n[G] [M] [O]";
  char const *s = string;

  s = read_stack(s, crate_list, 3);
  TEST_ASSERT_EQUAL('Z', crate_list[0]);
  TEST_ASSERT_EQUAL('\0', crate_list[1]);
  TEST_ASSERT_EQUAL('Z', crate_list[2]);
  TEST_ASSERT_EQUAL(string + strlen(string) + 1, s = read_stack(s, crate_list, 3));
  TEST_ASSERT_EQUAL('G', crate_list[0]);
  TEST_ASSERT_EQUAL('M', crate_list[1]);
  TEST_ASSERT_EQUAL('O', crate_list[2]);
}

/*--------------------------------------------
  get_stack */

static void test_one_stack(void)
{
  char myStack[] = "        [Z]\n";
  stack_t *stacks[3];
  get_stack(myStack, stacks, 3);
  TEST_ASSERT_EQUAL(NULL, stacks[0]);
  TEST_ASSERT_EQUAL(NULL, stacks[1]);
  TEST_ASSERT_EQUAL('Z', stacks[2]->crate);
}

static void test_simple_stack(void)
{
  char myStack[] = "        [Z]\n        [N]\n    [C] [D]\n    [M] [P]\n 1   2   3\n    \n";
  stack_t *stacks[3];
  stack_t lastStack;
  get_stack(myStack, stacks, 3);
  TEST_ASSERT_EQUAL(NULL, stacks[0]);
  TEST_ASSERT_EQUAL('C', stacks[1]->crate);
  TEST_ASSERT_EQUAL('Z', (lastStack = *stacks[2]).crate);
}

/*--------------------------------------------
  get_shuffle_command */
static void test_one_command(void)
{
  char myStack[] = " move 4 from 2 to 3";
  move_t move;
  TEST_ASSERT_GREATER_THAN_size_t(NULL, get_shuffle_command(myStack, &move, 6));
  TEST_ASSERT_EQUAL(4, move.amount);
  TEST_ASSERT_EQUAL(2, move.from);
  TEST_ASSERT_EQUAL(3, move.to);
}

static void test_two_commands(void)
{
  char myStack[] = " move 4 from 2 to 3\n move 5 from 3 to 4";
  char const *line = myStack;
  move_t move;
  TEST_ASSERT_GREATER_THAN_size_t(NULL, line = get_shuffle_command(line, &move, 6));
  TEST_ASSERT_EQUAL(4, move.amount);
  TEST_ASSERT_EQUAL(2, move.from);
  TEST_ASSERT_EQUAL(3, move.to);
  TEST_ASSERT_GREATER_THAN_size_t(NULL, get_shuffle_command(line, &move, 6));
  TEST_ASSERT_EQUAL(5, move.amount);
  TEST_ASSERT_EQUAL(3, move.from);
  TEST_ASSERT_EQUAL(4, move.to);
}

static void test_greater_ten(void)
{
  char myStack[] = " move 14 from 2 to 3\n move 5 from 3 to 4";
  char const *line = myStack;
  move_t move;
  TEST_ASSERT_GREATER_THAN_size_t(NULL, line = get_shuffle_command(line, &move, 6));
  TEST_ASSERT_EQUAL(14, move.amount);
  TEST_ASSERT_EQUAL(2, move.from);
  TEST_ASSERT_EQUAL(3, move.to);
  TEST_ASSERT_GREATER_THAN_size_t(NULL, get_shuffle_command(line, &move, 6));
  TEST_ASSERT_EQUAL(5, move.amount);
  TEST_ASSERT_EQUAL(3, move.from);
  TEST_ASSERT_EQUAL(4, move.to);
}

static void test_simple_move(void)
{
  move_t move;
  move.amount = 1;
  move.from = 3;
  move.to = 1;

  // depend on get_stack because handcrafting is hard
  char myStack[] = "        [Z]\n        [N]\n    [C] [D]\n    [M] [P]\n 1   2   3\n    \n";
  stack_t *stacks[3];
  get_stack(myStack, stacks, 3);
  shuffle_stacks(stacks, move);
  TEST_ASSERT_EQUAL('Z', stacks[0]->crate);
  TEST_ASSERT_EQUAL('C', stacks[1]->crate);
  TEST_ASSERT_EQUAL('N', stacks[2]->crate);
}

static void test_two_moves(void)
{
  move_t move;
  move.amount = 2;
  move.from = 3;
  move.to = 1;

  // depend on get_stack because handcrafting is hard
  char myStack[] = "        [Z]\n        [N]\n    [C] [D]\n    [M] [P]\n 1   2   3\n    \n";
  stack_t *stacks[3];
  get_stack(myStack, stacks, 3);
  shuffle_stacks(stacks, move);
  TEST_ASSERT_EQUAL('N', stacks[0]->crate);
  TEST_ASSERT_EQUAL('C', stacks[1]->crate);
  TEST_ASSERT_EQUAL('D', stacks[2]->crate);
}

int main(void) {
  UnityBegin("test_hamming.c");

  RUN_TEST(test_only_first);
  RUN_TEST(test_only_last);
  RUN_TEST(test_middle);
  RUN_TEST(test_all_but_middle);
  RUN_TEST(test_empty);
  RUN_TEST(test_invalid);
  RUN_TEST(test_pointer_return);

  RUN_TEST(test_one_stack);
  RUN_TEST(test_simple_stack);
  //RUN_TEST(test_simple_stack_l2);
  //RUN_TEST(test_simple_stack_l3);
  RUN_TEST(test_one_command);
  RUN_TEST(test_two_commands);
  RUN_TEST(test_greater_ten);

  RUN_TEST(test_simple_move);
  RUN_TEST(test_two_moves);
  return UnityEnd();
}
