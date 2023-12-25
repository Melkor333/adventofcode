#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include "../test-framework/unity.h"
#include "startofmarker.h"


void setUp(void)
{
}

void tearDown(void)
{
}

static void test_detect_package_start(void)
{
  char *text = "abcd";
  FILE *textfile;
  textfile = fmemopen(text, 5, "r");

  TEST_ASSERT_EQUAL(4, get_package_start(textfile, 4));
  fclose(textfile);
}

static void test_detect_package_start_after_something(void)
{
  char *text = "asdsudufdaabcd";
  FILE *textfile;
  textfile = fmemopen(text, 14, "r");

  TEST_ASSERT_EQUAL(10, get_package_start(textfile, 4));
  fclose(textfile);
}

static void test_message_start_by_examples(void)
{
  char *text = "mjqjpqmgbljsphdztnvjfqwrcgsmlb";
  FILE *textfile;
  textfile = fmemopen(text, 21, "r");

  TEST_ASSERT_EQUAL(19, get_package_start(textfile, 14));
  fclose(textfile);
}

static void test_2(void)
{
  char *text = "bvwbjplbgvbhsrlpgdmjqwftvncz";
  FILE *textfile;

  textfile = fmemopen(text, 24, "r");

  TEST_ASSERT_EQUAL(23, get_package_start(textfile, 14));
  fclose(textfile);
}

static void test_3(void)
{
  char *text = "mjqjpqmgbljsphdztnvjfqwrcgsmlb";
  FILE *textfile;
  textfile = fmemopen(text, 20, "r");

  TEST_ASSERT_EQUAL(19, get_package_start(textfile, 14));
  fclose(textfile);
}


int main(void) {
  UnityBegin("test_hamming.c");

  RUN_TEST(test_detect_package_start);
  RUN_TEST(test_detect_package_start_after_something);
  RUN_TEST(test_message_start_by_examples);
  RUN_TEST(test_2);
  RUN_TEST(test_3);
  return UnityEnd();
}
