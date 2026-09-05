#include <sys/stat.h>
#include "mocks.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "parsed_arguments.h"

#define SUITE_NAME "parsed_arguments"

static t_parsed_arguments *sut;

static void test_setup(void)
{
    vfs_mock_reset();
    const char *valid_args[] = { NULL };
    guarantee_stat_will_populate_stats_of_a_directory_type_file(".");
    sut = parse_arguments(0, valid_args);
}

static void test_teardown(void)
{
    reset_stat_guarantees();
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

static void should_return_default_values_if_arguments_are_NULL(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", "..", "."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { NULL };

    t_parsed_arguments *parsed_arguments = parse_arguments(0, arguments);
    const char * const *file_operands = parsed_arguments_get_file_operands(parsed_arguments);

    CU_ASSERT_PTR_NOT_NULL(parsed_arguments);
    CU_ASSERT_STRING_EQUAL(file_operands[0], ".");

    parsed_arguments_destroy(&parsed_arguments);
}

static void should_return_NULL_file_operands_if_NULL_parsed_arguments_are_passed(void)
{
    const char * const *file_operands = parsed_arguments_get_file_operands(NULL);

    CU_ASSERT_PTR_NULL(file_operands);
}

static void should_return_the_file_operands(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("Valid"),
        MOCK_FILE("file"),
        MOCK_FILE("operands"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "Valid", "file", "operands", NULL };
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
    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(NULL);

    CU_ASSERT_PTR_NULL(non_directory_file_operands);
}

static void should_return_the_non_directory_file_operands(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_SOCKET("1_socket"),
        MOCK_SYMLINK("2_symlink", "4_reg_file"),
        MOCK_CHAR_DEVICE("3_char_device"),
        MOCK_FILE("4_reg_file"),
        MOCK_FIFO("5_pipe"),
        MOCK_DIR("6_dir", ".", ".."),
        MOCK_BLOCK_DEVICE("7_block_device"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "1_socket", "2_symlink", "3_char_device", "4_reg_file", "5_pipe", "6_dir", "7_block_device", NULL };
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

static void should_return_NULL_directory_file_operands_if_NULL_parsed_arguments_are_passed(void)
{
    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(NULL);

    CU_ASSERT_PTR_NULL(directory_file_operands);
}

static void should_return_the_directory_file_operands(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_SOCKET("socket"),
        MOCK_SYMLINK("symlink", "reg_file"),
        MOCK_DIR("dir1", ".", ".."),
        MOCK_CHAR_DEVICE("char_device"),
        MOCK_FILE("reg_file"),
        MOCK_FIFO("pipe"),
        MOCK_DIR("dir2", ".", ".."),
        MOCK_BLOCK_DEVICE("block_device"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "socket", "symlink", "dir1", "char_device", "reg_file", "pipe", "dir2", "block_device", NULL };
    t_parsed_arguments *parsed_arguments = parse_arguments(8, args);

    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(parsed_arguments);

    CU_ASSERT_STRING_EQUAL(directory_file_operands[0], args[2]);
    CU_ASSERT_STRING_EQUAL(directory_file_operands[1], args[6]);
    CU_ASSERT_PTR_NULL(directory_file_operands[2]);

    parsed_arguments_destroy(&parsed_arguments);
}

static void should_return_false_for_multiple_file_operands_if_NULL_parsed_arguments_are_passed(void)
{
    const int has_multiple_file_operands = parsed_arguments_has_multiple_file_operands(NULL);

    CU_ASSERT_EQUAL(has_multiple_file_operands, 0);
}

static void should_return_false_for_multiple_file_operands_if_has_less_than_two(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { NULL };
    guarantee_stat_will_populate_stats_of_a_directory_type_file(".");
    t_parsed_arguments *parsed_arguments = parse_arguments(0, args);

    const int has_multiple_file_operands = parsed_arguments_has_multiple_file_operands(parsed_arguments);

    CU_ASSERT_EQUAL(has_multiple_file_operands, 0);

    parsed_arguments_destroy(&parsed_arguments);
}

static void should_return_true_for_multiple_file_operands_if_has_equal_or_more_than_two(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", ".."),
        MOCK_FILE("file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "file", "dir", NULL };
    t_parsed_arguments *parsed_arguments = parse_arguments(2, args);

    const int has_multiple_file_operands = parsed_arguments_has_multiple_file_operands(parsed_arguments);

    CU_ASSERT_EQUAL(has_multiple_file_operands, 1);

    parsed_arguments_destroy(&parsed_arguments);
}

static void should_return_false_for_has_directory_file_operands_if_NULL_parsed_arguments_are_passed(void)
{
    const int has_directory_file_operands = parsed_arguments_has_directory_file_operands(NULL);

    CU_ASSERT_EQUAL(has_directory_file_operands, 0);
}

static void should_return_false_for_has_directory_file_operands_if_it_does_not_have_any(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_FILE("file3"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "file", "file2", "file3", NULL };
    t_parsed_arguments *parsed_arguments = parse_arguments(3, args);

    const int has_directory_file_operands = parsed_arguments_has_directory_file_operands(parsed_arguments);

    CU_ASSERT_EQUAL(has_directory_file_operands, 0);

    parsed_arguments_destroy(&parsed_arguments);
}

static void should_return_true_for_has_directory_file_operands_if_it_has_at_least_one(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "dir", NULL };
    t_parsed_arguments *parsed_arguments = parse_arguments(1, args);

    const int has_directory_file_operands = parsed_arguments_has_directory_file_operands(parsed_arguments);

    CU_ASSERT_EQUAL(has_directory_file_operands, 1);

    parsed_arguments_destroy(&parsed_arguments);
}

static void should_sort_the_non_directory_file_operands_by_ascii_by_default(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("a"),
        MOCK_FILE("2file"),
        MOCK_FILE("_file"),
        MOCK_FILE("f"),
        MOCK_FILE(".hidden_file"),
        MOCK_FILE("FILE"),
        MOCK_DIR("_DIR", "~file", ".", "file1", "..", "-file"),
        MOCK_DIR(".hiddir", "file2", "..", "FILE", ".", "symlink"),
        MOCK_DIR("dir", "..", "2FILE", ".", "file3"),
        MOCK_FILE("_DIR/~file"),
        MOCK_FILE("_DIR/file1"),
        MOCK_FILE("_DIR/-file"),
        MOCK_FILE(".hiddir/file2"),
        MOCK_FILE(".hiddir/FILE"),
        MOCK_SYMLINK(".hiddir/symlink", ".hiddir/FILE"),
        MOCK_FILE("dir/2FILE"),
        MOCK_FILE("dir/file3"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "a", "2file", "_DIR", ".hiddir", "_file", "f", "dir", ".hidden_file", "FILE", NULL };
    t_parsed_arguments *parsed_arguments = parse_arguments(9, args);

    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(parsed_arguments);

    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[0], args[7]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[1], args[1]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[2], args[8]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[3], args[4]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[4], args[0]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[5], args[5]);
    CU_ASSERT_PTR_NULL(non_directory_file_operands[6]);

    parsed_arguments_destroy(&parsed_arguments);
}

static void should_sort_the_directory_file_operands_by_ascii_by_default(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("a"),
        MOCK_FILE("2file"),
        MOCK_FILE("_file"),
        MOCK_FILE("f"),
        MOCK_FILE(".hidden_file"),
        MOCK_FILE("FILE"),
        MOCK_DIR("_DIR", "~file", ".", "file1", "..", "-file"),
        MOCK_DIR(".hiddir", "file2", "..", "FILE", ".", "symlink"),
        MOCK_DIR("dir", "..", "2FILE", ".", "file3"),
        MOCK_FILE("_DIR/~file"),
        MOCK_FILE("_DIR/file1"),
        MOCK_FILE("_DIR/-file"),
        MOCK_FILE(".hiddir/file2"),
        MOCK_FILE(".hiddir/FILE"),
        MOCK_SYMLINK(".hiddir/symlink", ".hiddir/FILE"),
        MOCK_FILE("dir/2FILE"),
        MOCK_FILE("dir/file3"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "a", "2file", "_DIR", ".hiddir", "_file", "f", "dir", ".hidden_file", "FILE", NULL };
    t_parsed_arguments *parsed_arguments = parse_arguments(9, args);

    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(parsed_arguments);

    CU_ASSERT_STRING_EQUAL(directory_file_operands[0], args[3]);
    CU_ASSERT_STRING_EQUAL(directory_file_operands[1], args[2]);
    CU_ASSERT_STRING_EQUAL(directory_file_operands[2], args[6]);
    CU_ASSERT_PTR_NULL(directory_file_operands[3]);

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
        CU_add_test(suite, "should_be_created_with_default_values_if_arguments_are_NULL", should_return_default_values_if_arguments_are_NULL);
        CU_add_test(suite, "should_return_NULL_file_operands_if_NULL_parsed_arguments_are_passed", should_return_NULL_file_operands_if_NULL_parsed_arguments_are_passed);
        CU_add_test(suite, "should_return_the_file_operands", should_return_the_file_operands);
        CU_add_test(suite, "should_return_NULL_non_directory_file_operands_if_NULL_parsed_arguments_are_passed", should_return_NULL_non_directory_file_operands_if_NULL_parsed_arguments_are_passed);
        CU_add_test(suite, "should_return_the_non_directory_file_operands", should_return_the_non_directory_file_operands);
        CU_add_test(suite, "should_return_NULL_directory_file_operands_if_NULL_parsed_arguments_are_passed", should_return_NULL_directory_file_operands_if_NULL_parsed_arguments_are_passed);
        CU_add_test(suite, "should_return_the_directory_file_operands", should_return_the_directory_file_operands);
        CU_add_test(suite, "should_return_false_for_multiple_file_operands_if_NULL_parsed_arguments_are_passed", should_return_false_for_multiple_file_operands_if_NULL_parsed_arguments_are_passed);
        CU_add_test(suite, "should_return_false_for_multiple_file_operands_if_has_less_than_two", should_return_false_for_multiple_file_operands_if_has_less_than_two);
        CU_add_test(suite, "should_return_true_for_multiple_file_operands_if_has_equal_or_more_than_two", should_return_true_for_multiple_file_operands_if_has_equal_or_more_than_two);
        CU_add_test(suite, "should_return_false_for_has_directory_file_operands_if_NULL_parsed_arguments_are_passed", should_return_false_for_has_directory_file_operands_if_NULL_parsed_arguments_are_passed);
        CU_add_test(suite, "should_return_false_for_has_directory_file_operands_if_it_does_not_have_any", should_return_false_for_has_directory_file_operands_if_it_does_not_have_any);
        CU_add_test(suite, "should_return_true_for_has_directory_file_operands_if_it_has_at_least_one", should_return_true_for_has_directory_file_operands_if_it_has_at_least_one);
        CU_add_test(suite, "should_sort_the_non_directory_file_operands_by_ascii_by_default", should_sort_the_non_directory_file_operands_by_ascii_by_default);
        CU_add_test(suite, "should_sort_the_directory_file_operands_by_ascii_by_default", should_sort_the_directory_file_operands_by_ascii_by_default);
    }
}
