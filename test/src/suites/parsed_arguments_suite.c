#include <sys/stat.h>
#include "libft.h"
#include "mocks.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "parsed_arguments.h"

#define SUITE_NAME "parsed_arguments"

static t_parsed_arguments *sut;

static void test_setup(void)
{
    const char *valid_args[] = { NULL };
    sut = parse_arguments(0, valid_args);
}

static void test_teardown(void)
{
    parsed_arguments_destroy(&sut);
}

static void should_be_created_correctly(void)
{
    CU_ASSERT_PTR_NOT_NULL(sut);
}

static void should_be_destroyed_correctly(void)
{
    parsed_arguments_destroy(&sut);

    CU_ASSERT_PTR_NULL(sut);
}

static void should_not_fail_to_destroy_when_a_null_pointer_is_passed(void)
{
    parsed_arguments_destroy(NULL);
}

static void should_not_fail_to_destroy_if_its_already_null(void)
{
    parsed_arguments_destroy(&sut);
    parsed_arguments_destroy(&sut);
}

static void should_return_NULL_if_num_of_arguments_is_negative(void)
{
    const char *args[] = { "valid", "args", NULL };
    t_parsed_arguments *parsed_arguments = parse_arguments(-1, args);

    CU_ASSERT_PTR_NULL(parsed_arguments);

    parsed_arguments_destroy(&parsed_arguments);
}

static void should_return_NULL_if_arguments_are_NULL(void)
{
    t_parsed_arguments *parsed_arguments = parse_arguments(1, NULL);

    CU_ASSERT_PTR_NULL(parsed_arguments);

    parsed_arguments_destroy(&parsed_arguments);
}

static void should_return_NULL_file_operands_if_NULL_parsed_arguments_are_passed(void)
{
    const char * const *file_operands = parsed_arguments_get_file_operands(NULL);

    CU_ASSERT_PTR_NULL(file_operands);
}

static void should_return_the_file_operands(void)
{
    const char *args[] = { "valid", "file", "operands", NULL };
    const unsigned int args_types[] = { S_IFREG, S_IFREG, S_IFREG, 0 };
    guarantee_stat_will_populate_stats_of_N_file_types_for_paths(args, args_types);
    t_parsed_arguments *parsed_arguments = parse_arguments(3, args);

    const char * const *file_operands = parsed_arguments_get_file_operands(parsed_arguments);

    CU_ASSERT_STRING_EQUAL(file_operands[0], args[0]);
    CU_ASSERT_STRING_EQUAL(file_operands[1], args[1]);
    CU_ASSERT_STRING_EQUAL(file_operands[2], args[2]);
    CU_ASSERT_PTR_NULL(file_operands[3]);

    parsed_arguments_destroy(&parsed_arguments);
}

static void should_return_NULL_non_directory_file_operands_if_NULL_parsed_arguments_are_passed(void)
{
    const char * const *file_operands = parsed_arguments_get_non_directory_file_operands(NULL);

    CU_ASSERT_PTR_NULL(file_operands);
}

static void should_return_the_non_directory_file_operands(void)
{
    const char *args[] = { "socket", "symlink", "char_device", "reg_file", "pipe", "dir", "block_device", NULL };
    const unsigned int args_types[] = { S_IFSOCK, S_IFLNK, S_IFCHR, S_IFREG, S_IFIFO, S_IFDIR, S_IFBLK, 0 };
    guarantee_stat_will_populate_stats_of_N_file_types_for_paths(args, args_types);
    t_parsed_arguments *parsed_arguments = parse_arguments(7, args);

    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(parsed_arguments);

    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[0], args[0]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[1], args[1]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[2], args[2]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[3], args[3]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[4], args[4]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[5], args[6]);
    CU_ASSERT_PTR_NULL(non_directory_file_operands[6]);

    parsed_arguments_destroy(&parsed_arguments);
}

static void should_return_the_directory_file_operands(void)
{
    const char *args[] = { "socket", "symlink", "dir1", "char_device", "reg_file", "pipe", "dir2", "block_device", NULL };
    const unsigned int args_types[] = { S_IFSOCK, S_IFLNK, S_IFDIR, S_IFCHR, S_IFREG, S_IFIFO, S_IFDIR, S_IFBLK, 0 };
    guarantee_stat_will_populate_stats_of_N_file_types_for_paths(args, args_types);
    t_parsed_arguments *parsed_arguments = parse_arguments(8, args);

    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(parsed_arguments);

    CU_ASSERT_STRING_EQUAL(directory_file_operands[0], args[2]);
    CU_ASSERT_STRING_EQUAL(directory_file_operands[1], args[6]);
    CU_ASSERT_PTR_NULL(directory_file_operands[2]);

    parsed_arguments_destroy(&parsed_arguments);
}

void register_parsed_arguments_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, test_teardown);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_be_created_correctly", should_be_created_correctly);
        CU_add_test(suite, "should_be_destroyed_correctly", should_be_destroyed_correctly);
        CU_add_test(suite, "should_not_fail_to_destroy_when_a_null_pointer_is_passed", should_not_fail_to_destroy_when_a_null_pointer_is_passed);
        CU_add_test(suite, "should_not_fail_to_destroy_if_its_already_null", should_not_fail_to_destroy_if_its_already_null);
        CU_add_test(suite, "should_return_NULL_if_num_of_arguments_is_negative", should_return_NULL_if_num_of_arguments_is_negative);
        CU_add_test(suite, "should_return_NULL_if_arguments_are_NULL", should_return_NULL_if_arguments_are_NULL);
        CU_add_test(suite, "should_return_NULL_file_operands_if_NULL_parsed_arguments_are_passed", should_return_NULL_file_operands_if_NULL_parsed_arguments_are_passed);
        CU_add_test(suite, "should_return_the_file_operands", should_return_the_file_operands);
        CU_add_test(suite, "should_return_NULL_non_directory_file_operands_if_NULL_parsed_arguments_are_passed", should_return_NULL_non_directory_file_operands_if_NULL_parsed_arguments_are_passed);
        CU_add_test(suite, "should_return_the_non_directory_file_operands", should_return_the_non_directory_file_operands);
        CU_add_test(suite, "should_return_the_directory_file_operands", should_return_the_directory_file_operands);
    }
}
