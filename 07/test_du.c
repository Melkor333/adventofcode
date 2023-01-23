#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include "../test-framework/unity.h"
#include "du.h"

void setUp(void)
{
}

void tearDown(void)
{
}

static void test_ls(void)
{
  char *text = "$ ls";

  TEST_ASSERT_EQUAL(0, get_ls(text));
}

static void test_no_ls(void)
{
  char *text = "$ cd /";

  TEST_ASSERT_NOT_EQUAL_CHAR(0, get_ls(text));
}

static void test_path_simple(void)
{
  char *text = "$ cd abac";
  char name[10];

  TEST_ASSERT_EQUAL(1, cd_path(text, name));
  TEST_ASSERT_EQUAL_STRING(name, "abac");
}

static void test_path_dot(void)
{
  char *text = "$ cd ab.ac";
  char name[10];

  TEST_ASSERT_EQUAL(1, cd_path(text, name));
  TEST_ASSERT_EQUAL_STRING(name, "ab.ac");
}

static void test_path_slash(void)
{
  char *text = "cd /";
  char name[10];

  TEST_ASSERT_EQUAL(1, cd_path(text, name));
  TEST_ASSERT_EQUAL_STRING(name, "/");
}

static void test_dir_simple(void)
{
  char *text = "dir abac";
  char name[10];

  TEST_ASSERT_EQUAL(1, get_dir(text, name));
  TEST_ASSERT_EQUAL_STRING("abac", name);
}

static void test_dir_dot(void)
{
  char *text = "dir ab.ac";
  char name[10];

  TEST_ASSERT_EQUAL(1, get_dir(text, name));
  TEST_ASSERT_EQUAL_STRING(name, "ab.ac");
}

static void test_file_simple(void)
{
  char *text = "12353 abac";
  char name[10];
  int size;

  TEST_ASSERT_EQUAL(2, get_file(text, name, &size));
  TEST_ASSERT_EQUAL_STRING(name, "abac");
  TEST_ASSERT_EQUAL(size, 12353);
}

static void test_file_dot(void)
{
  char *text = "12444 ab.ac";
  char name[10];
  int size;

  TEST_ASSERT_EQUAL(2, get_file(text, name, &size));
  TEST_ASSERT_EQUAL_STRING(name, "ab.ac");
  TEST_ASSERT_EQUAL(size, 12444);
}

int main(void) {
  UnityBegin("test_hamming.c");

  RUN_TEST(test_ls);
  RUN_TEST(test_no_ls);
  RUN_TEST(test_path_simple);
  RUN_TEST(test_path_dot);
  RUN_TEST(test_path_slash);
  RUN_TEST(test_dir_simple);
  RUN_TEST(test_dir_dot);
  RUN_TEST(test_file_simple);
  RUN_TEST(test_file_dot);
  return UnityEnd();
}
