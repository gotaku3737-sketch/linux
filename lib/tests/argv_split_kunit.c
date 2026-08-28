// SPDX-License-Identifier: GPL-2.0-only
/*
 * KUnit test for argv_split()
 */

#include <kunit/test.h>
#include <linux/string.h>
#include <linux/slab.h>

static void test_argv_split_normal(struct kunit *test)
{
	const char *str = "a b c";
	int argc;
	char **argv;

	argv = argv_split(GFP_KERNEL, str, &argc);
	KUNIT_ASSERT_NOT_ERR_OR_NULL(test, argv);
	KUNIT_EXPECT_EQ(test, argc, 3);
	KUNIT_EXPECT_STREQ(test, argv[0], "a");
	KUNIT_EXPECT_STREQ(test, argv[1], "b");
	KUNIT_EXPECT_STREQ(test, argv[2], "c");
	KUNIT_EXPECT_NULL(test, argv[3]);

	argv_free(argv);
}

static void test_argv_split_empty(struct kunit *test)
{
	const char *str = "";
	int argc;
	char **argv;

	argv = argv_split(GFP_KERNEL, str, &argc);
	KUNIT_ASSERT_NOT_ERR_OR_NULL(test, argv);
	KUNIT_EXPECT_EQ(test, argc, 0);
	KUNIT_EXPECT_NULL(test, argv[0]);

	argv_free(argv);
}

static void test_argv_split_spaces(struct kunit *test)
{
	const char *str = "   ";
	int argc;
	char **argv;

	argv = argv_split(GFP_KERNEL, str, &argc);
	KUNIT_ASSERT_NOT_ERR_OR_NULL(test, argv);
	KUNIT_EXPECT_EQ(test, argc, 0);
	KUNIT_EXPECT_NULL(test, argv[0]);

	argv_free(argv);
}

static void test_argv_split_leading_trailing(struct kunit *test)
{
	const char *str = "  a b c  ";
	int argc;
	char **argv;

	argv = argv_split(GFP_KERNEL, str, &argc);
	KUNIT_ASSERT_NOT_ERR_OR_NULL(test, argv);
	KUNIT_EXPECT_EQ(test, argc, 3);
	KUNIT_EXPECT_STREQ(test, argv[0], "a");
	KUNIT_EXPECT_STREQ(test, argv[1], "b");
	KUNIT_EXPECT_STREQ(test, argv[2], "c");
	KUNIT_EXPECT_NULL(test, argv[3]);

	argv_free(argv);
}

static void test_argv_split_multiple_spaces(struct kunit *test)
{
	const char *str = "a   b  c";
	int argc;
	char **argv;

	argv = argv_split(GFP_KERNEL, str, &argc);
	KUNIT_ASSERT_NOT_ERR_OR_NULL(test, argv);
	KUNIT_EXPECT_EQ(test, argc, 3);
	KUNIT_EXPECT_STREQ(test, argv[0], "a");
	KUNIT_EXPECT_STREQ(test, argv[1], "b");
	KUNIT_EXPECT_STREQ(test, argv[2], "c");
	KUNIT_EXPECT_NULL(test, argv[3]);

	argv_free(argv);
}

static void test_argv_split_null_argcp(struct kunit *test)
{
	const char *str = "a b";
	char **argv;

	argv = argv_split(GFP_KERNEL, str, NULL);
	KUNIT_ASSERT_NOT_ERR_OR_NULL(test, argv);
	KUNIT_EXPECT_STREQ(test, argv[0], "a");
	KUNIT_EXPECT_STREQ(test, argv[1], "b");
	KUNIT_EXPECT_NULL(test, argv[2]);

	argv_free(argv);
}

static struct kunit_case argv_split_test_cases[] = {
	KUNIT_CASE(test_argv_split_normal),
	KUNIT_CASE(test_argv_split_empty),
	KUNIT_CASE(test_argv_split_spaces),
	KUNIT_CASE(test_argv_split_leading_trailing),
	KUNIT_CASE(test_argv_split_multiple_spaces),
	KUNIT_CASE(test_argv_split_null_argcp),
	{}
};

static struct kunit_suite argv_split_test_suite = {
	.name = "argv_split",
	.test_cases = argv_split_test_cases,
};

kunit_test_suite(argv_split_test_suite);

MODULE_LICENSE("GPL");
