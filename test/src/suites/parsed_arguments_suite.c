#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "mocks.h"
#include "parsed_arguments.h"

#define SUITE_NAME "parsed_arguments"

static t_result *parsed_arguments_result;
static t_parsed_arguments *sut;

static void test_setup(void)
{
    reset_printing_buffer();
    vfs_mock_reset();
}

static void test_teardown(void)
{
    result_destroy(&parsed_arguments_result);
    parsed_arguments_destroy(&sut);
}

static void get_parsed_arguments_result(const int argc, const char **args)
{
    parsed_arguments_result = parse_arguments(argc, args);
    sut = result_get_value(parsed_arguments_result);
}

static void should_be_created_correctly(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", "..", "."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *valid_args[] = { ".", NULL };

    get_parsed_arguments_result(1, valid_args);
    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(sut);
    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(sut);

    CU_ASSERT_PTR_NOT_NULL(sut);
    CU_ASSERT_FALSE(parsed_arguments_has_any_option(sut));
    CU_ASSERT_PTR_NULL(non_directory_file_operands[0]);
    CU_ASSERT_STRING_EQUAL(directory_file_operands[0], ".");
    CU_ASSERT_PTR_NULL(directory_file_operands[1]);
    CU_ASSERT_TRUE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_be_destroyed_correctly(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", "..", "."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *valid_args[] = { NULL };
    get_parsed_arguments_result(0, valid_args);

    parsed_arguments_destroy(&sut);

    CU_ASSERT_PTR_NULL(sut);
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_not_fail_to_destroy_when_a_null_pointer_is_passed(void)
{
    parsed_arguments_destroy(NULL);

    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_not_fail_to_destroy_if_its_already_null(void)
{
    t_parsed_arguments *invalid = NULL;

    parsed_arguments_destroy(&invalid);

    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_NULL_if_num_of_arguments_is_negative(void)
{
    const char *args[] = { "valid", "args", NULL };
    get_parsed_arguments_result(-1, args);

    CU_ASSERT_PTR_NULL(sut);
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_NULL_if_arguments_are_NULL(void)
{
    get_parsed_arguments_result(1, NULL);

    CU_ASSERT_PTR_NULL(sut);
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_default_values_if_arguments_are_NULL(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", "..", "."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { NULL };

    get_parsed_arguments_result(0, arguments);
    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(sut);
    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(sut);

    CU_ASSERT_PTR_NOT_NULL(sut);
    CU_ASSERT_FALSE(parsed_arguments_has_any_option(sut));
    CU_ASSERT_PTR_NULL(non_directory_file_operands[0]);
    CU_ASSERT_STRING_EQUAL(directory_file_operands[0], ".");
    CU_ASSERT_PTR_NULL(directory_file_operands[1]);
    CU_ASSERT_TRUE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_NULL_non_directory_file_operands_if_NULL_parsed_arguments_are_passed(void)
{
    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(NULL);

    CU_ASSERT_PTR_NULL(non_directory_file_operands);
    CU_ASSERT(verify_that_no_error_was_printed());
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
    get_parsed_arguments_result(7, args);

    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(sut);

    CU_ASSERT_FALSE(parsed_arguments_has_any_option(sut));
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[0], args[0]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[1], args[1]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[2], args[2]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[3], args[3]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[4], args[4]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[5], args[6]);
    CU_ASSERT_PTR_NULL(non_directory_file_operands[6]);
    CU_ASSERT_TRUE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_NULL_directory_file_operands_if_NULL_parsed_arguments_are_passed(void)
{
    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(NULL);

    CU_ASSERT_PTR_NULL(directory_file_operands);
    CU_ASSERT(verify_that_no_error_was_printed());
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
    get_parsed_arguments_result(8, args);

    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(sut);

    CU_ASSERT_FALSE(parsed_arguments_has_any_option(sut));
    CU_ASSERT_STRING_EQUAL(directory_file_operands[0], args[2]);
    CU_ASSERT_STRING_EQUAL(directory_file_operands[1], args[6]);
    CU_ASSERT_PTR_NULL(directory_file_operands[2]);
    CU_ASSERT_TRUE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_false_for_has_any_option_if_NULL_parsed_arguments_are_passed(void)
{
    const int has_any_option = parsed_arguments_has_any_option(NULL);

    CU_ASSERT_FALSE(has_any_option);
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_false_for_has_any_option_if_at_no_option_was_specified(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", ".."),
        MOCK_FILE("file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "file", "dir", NULL };
    get_parsed_arguments_result(2, args);

    const int has_any_option = parsed_arguments_has_any_option(sut);

    CU_ASSERT_FALSE(has_any_option);
    CU_ASSERT_TRUE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_true_for_has_any_option_if_at_least_one_option_was_specified(void)
{
    const char *args[] = { "-R", NULL };
    get_parsed_arguments_result(1, args);

    const int has_any_option = parsed_arguments_has_any_option(sut);

    CU_ASSERT_TRUE(has_any_option);
    CU_ASSERT_TRUE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_false_for_has_option_if_NULL_parsed_arguments_are_passed(void)
{
    const int has_option = parsed_arguments_has_option(NULL, OPTIONS_RECURSIVE);

    CU_ASSERT_FALSE(has_option);
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_false_for_has_option_if_the_specified_option_was_NOT_parsed(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "dir", "-l", NULL };
    get_parsed_arguments_result(2, args);

    const int has_option = parsed_arguments_has_option(sut, OPTIONS_RECURSIVE);

    CU_ASSERT_FALSE(has_option);
    CU_ASSERT_TRUE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_true_for_has_option_if_the_specified_option_was_parsed(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "file", "-R", NULL };
    get_parsed_arguments_result(2, args);

    const int has_option = parsed_arguments_has_option(sut, OPTIONS_RECURSIVE);

    CU_ASSERT_TRUE(has_option);
    CU_ASSERT_TRUE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_false_for_multiple_file_operands_if_NULL_parsed_arguments_are_passed(void)
{
    const int has_multiple_file_operands = parsed_arguments_has_multiple_file_operands(NULL);

    CU_ASSERT_FALSE(has_multiple_file_operands);
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_false_for_multiple_file_operands_if_has_less_than_two(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { NULL };
    get_parsed_arguments_result(0, args);

    const int has_multiple_file_operands = parsed_arguments_has_multiple_file_operands(sut);

    CU_ASSERT_FALSE(has_multiple_file_operands);
    CU_ASSERT_TRUE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT(verify_that_no_error_was_printed());
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
    get_parsed_arguments_result(2, args);

    const int has_multiple_file_operands = parsed_arguments_has_multiple_file_operands(sut);

    CU_ASSERT_TRUE(has_multiple_file_operands);
    CU_ASSERT_TRUE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_false_for_has_directory_file_operands_if_NULL_parsed_arguments_are_passed(void)
{
    const int has_directory_file_operands = parsed_arguments_has_directory_file_operands(NULL);

    CU_ASSERT_FALSE(has_directory_file_operands);
    CU_ASSERT(verify_that_no_error_was_printed());
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
    get_parsed_arguments_result(3, args);

    const int has_directory_file_operands = parsed_arguments_has_directory_file_operands(sut);

    CU_ASSERT_FALSE(has_directory_file_operands);
    CU_ASSERT_TRUE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_true_for_has_directory_file_operands_if_it_has_at_least_one(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "dir", NULL };
    get_parsed_arguments_result(1, args);

    const int has_directory_file_operands = parsed_arguments_has_directory_file_operands(sut);

    CU_ASSERT_TRUE(has_directory_file_operands);
    CU_ASSERT_TRUE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_false_for_has_mixed_types_file_operands_if_it_null_parsed_arguments_are_specified(void)
{
    CU_ASSERT_FALSE(parsed_arguments_has_mixed_types_file_operands(NULL));
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_false_for_has_mixed_types_file_operands_if_it_does_not_have_at_least_one_non_dir_and_one_dir(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "file2", "file", NULL };
    get_parsed_arguments_result(2, args);

    const int has_mixed_types_file_operands = parsed_arguments_has_mixed_types_file_operands(sut);

    CU_ASSERT_FALSE(has_mixed_types_file_operands);
    CU_ASSERT_TRUE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_true_for_has_mixed_types_file_operands_if_it_does_have_at_least_one_non_dir_and_one_dir(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file"),
        MOCK_DIR("dir", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "dir", "file", NULL };
    get_parsed_arguments_result(2, args);

    const int has_mixed_types_file_operands = parsed_arguments_has_mixed_types_file_operands(sut);

    CU_ASSERT_TRUE(has_mixed_types_file_operands);
    CU_ASSERT_TRUE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_sort_the_non_directory_file_operands_by_ascii_by_default(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("a"),
        MOCK_FILE("2file"),
        MOCK_FILE("_Reg"),
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

    const char *args[] = { "a", "2file", "_DIR", ".hiddir", "_Reg", "f", "dir", ".hidden_file", "FILE", NULL };
    get_parsed_arguments_result(9, args);

    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(sut);

    CU_ASSERT_FALSE(parsed_arguments_has_any_option(sut));
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[0], args[7]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[1], args[1]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[2], args[8]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[3], args[4]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[4], args[0]);
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[5], args[5]);
    CU_ASSERT_PTR_NULL(non_directory_file_operands[6]);
    CU_ASSERT_TRUE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_sort_the_directory_file_operands_by_ascii_by_default(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("a"),
        MOCK_FILE("2file"),
        MOCK_FILE("_Reg"),
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

    const char *args[] = { "a", "2file", "_DIR", ".hiddir", "_Reg", "f", "dir", ".hidden_file", "FILE", NULL };
    get_parsed_arguments_result(9, args);

    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(sut);

    CU_ASSERT_FALSE(parsed_arguments_has_any_option(sut));
    CU_ASSERT_STRING_EQUAL(directory_file_operands[0], args[3]);
    CU_ASSERT_STRING_EQUAL(directory_file_operands[1], args[2]);
    CU_ASSERT_STRING_EQUAL(directory_file_operands[2], args[6]);
    CU_ASSERT_PTR_NULL(directory_file_operands[3]);
    CU_ASSERT_TRUE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_parse_the_recursive_option(void)
{
    const char *arguments[] = { "-R", NULL };

    get_parsed_arguments_result(1, arguments);
    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(sut);
    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(sut);

    CU_ASSERT_TRUE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT_TRUE(parsed_arguments_has_option(sut, OPTIONS_RECURSIVE));
    CU_ASSERT_PTR_NULL(non_directory_file_operands[0]);
    CU_ASSERT_PTR_NULL(directory_file_operands[0]);
    CU_ASSERT(verify_that_no_error_was_printed());
}

// -------

static void should_be_created_correctly_even_if_current_directory_is_invalid_if_no_parameters_were_specified(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR_ACCESS_ERROR(EACCES, ".", "..", ".", "file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *valid_args[] = { NULL };

    get_parsed_arguments_result(0, valid_args);
    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(sut);
    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(sut);

    CU_ASSERT_PTR_NOT_NULL(sut);
    CU_ASSERT_TRUE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT_FALSE(parsed_arguments_has_any_option(sut));
    CU_ASSERT_PTR_NULL(non_directory_file_operands[0]);
    CU_ASSERT_STRING_EQUAL(directory_file_operands[0], ".");
    CU_ASSERT_PTR_NULL(directory_file_operands[1]);
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_be_created_correctly_even_if_current_directory_is_invalid_and_was_specified(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR_ACCESS_ERROR(EACCES, ".", "..", ".", "file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *valid_args[] = { ".", NULL };

    get_parsed_arguments_result(1, valid_args);
    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(sut);
    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(sut);

    CU_ASSERT_PTR_NOT_NULL(sut);
    CU_ASSERT_TRUE(result_has_failed(parsed_arguments_result));
    CU_ASSERT_FALSE(parsed_arguments_has_any_option(sut));
    CU_ASSERT_PTR_NULL(non_directory_file_operands[0]);
    CU_ASSERT_PTR_NULL(directory_file_operands[0]);
    CU_ASSERT(verify_that_the_error_printed_is("ft_ls: cannot access '%s': %s\n", valid_args[0], strerror(EACCES)));
}

static void should_be_created_correctly_even_if_the_specified_argument_cannot_be_accessed(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_ACCESS_ERROR(ENOENT, "notExistent"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *valid_args[] = { "notExistent", NULL };

    get_parsed_arguments_result(1, valid_args);
    const char * const *non_directory_file_operands = parsed_arguments_get_directory_file_operands(sut);
    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(sut);

    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n",
        "notExistent", strerror(ENOENT)
    ));
    CU_ASSERT_FALSE(parsed_arguments_has_any_option(sut));
    CU_ASSERT_PTR_NULL(non_directory_file_operands[0]);
    CU_ASSERT_PTR_NULL(directory_file_operands[0]);
    CU_ASSERT_FALSE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT_PTR_NOT_NULL(sut);
}

static void should_be_created_correctly_even_if_one_argument_cannot_be_accessed(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_ACCESS_ERROR(ENOENT, "notExistent"),
        MOCK_FILE("file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *valid_args[] = { "notExistent", "file", NULL };

    get_parsed_arguments_result(2, valid_args);
    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(sut);
    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(sut);

    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n",
        "notExistent", strerror(ENOENT)
    ));
    CU_ASSERT_FALSE(parsed_arguments_has_any_option(sut));
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[0], "file");
    CU_ASSERT_PTR_NULL(non_directory_file_operands[1]);
    CU_ASSERT_PTR_NULL(directory_file_operands[0]);
    CU_ASSERT_FALSE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT_PTR_NOT_NULL(sut);
}

static void should_be_created_correctly_even_if_one_argument_cannot_be_accessed_with_multiple_mixed_file_operands(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_ACCESS_ERROR(ELOOP, "linkloop"),
        MOCK_FILE("file"),
        MOCK_SYMLINK("filelink", "file"),
        MOCK_SYMLINK("dirlink", "dir1"),
        MOCK_DIR("dir1", ".", "..", "file", "file2"),
        MOCK_DIR("empty", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *valid_args[] = { "file", "empty", "linkloop", "filelink", "dir1", "dirlink", NULL };

    get_parsed_arguments_result(6, valid_args);
    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(sut);
    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(sut);

    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n",
        "linkloop", strerror(ELOOP)
    ));
    CU_ASSERT_FALSE(parsed_arguments_has_any_option(sut));
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[0], "file");
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[1], "filelink");
    CU_ASSERT_PTR_NULL(non_directory_file_operands[2]);
    CU_ASSERT_STRING_EQUAL(directory_file_operands[0], "dir1");
    CU_ASSERT_STRING_EQUAL(directory_file_operands[1], "dirlink");
    CU_ASSERT_STRING_EQUAL(directory_file_operands[2], "empty");
    CU_ASSERT_PTR_NULL(directory_file_operands[3]);
    CU_ASSERT_FALSE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT_PTR_NOT_NULL(sut);
}

static void should_be_created_correctly_even_if_the_specified_arguments_cannot_be_accessed(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_ACCESS_ERROR(ENOENT, "notExistent"),
        MOCK_FILE_ACCESS_ERROR(ENOTDIR, "notADir/"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *valid_args[] = { "notExistent", "notADir/", NULL };

    get_parsed_arguments_result(2, valid_args);
    const char * const *non_directory_file_operands = parsed_arguments_get_directory_file_operands(sut);
    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(sut);

    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot access '%s': %s\n",
        "notExistent", strerror(ENOENT),
        "notADir/", strerror(ENOTDIR)
    ));
    CU_ASSERT_FALSE(parsed_arguments_has_any_option(sut));
    CU_ASSERT_PTR_NULL(non_directory_file_operands[0]);
    CU_ASSERT_PTR_NULL(directory_file_operands[0]);
    CU_ASSERT_FALSE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT_PTR_NOT_NULL(sut);
}

static void should_be_created_correctly_even_if_some_arguments_cannot_be_accessed(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_ACCESS_ERROR(EACCES, "noPerms"),
        MOCK_FILE_ACCESS_ERROR(EFAULT, "badAddress"),
        MOCK_FILE_ACCESS_ERROR(ENOMEM, "noMem"),
        MOCK_FILE("file"),
        MOCK_SYMLINK("filelink", "file"),
        MOCK_SYMLINK("dirlink", "dir1"),
        MOCK_DIR("dir1", ".", "..", "file", "file2"),
        MOCK_DIR("empty", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *valid_args[] = { "noPerms", "file", "empty", "filelink", "dir1", "badAddress", "dirlink", "noMem", NULL };

    get_parsed_arguments_result(8, valid_args);
    const char * const *non_directory_file_operands = parsed_arguments_get_non_directory_file_operands(sut);
    const char * const *directory_file_operands = parsed_arguments_get_directory_file_operands(sut);

    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot access '%s': %s\n",
        "noPerms", strerror(EACCES),
        "badAddress", strerror(EFAULT),
        "noMem", strerror(ENOMEM)
    ));
    CU_ASSERT_FALSE(parsed_arguments_has_any_option(sut));
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[0], "file");
    CU_ASSERT_STRING_EQUAL(non_directory_file_operands[1], "filelink");
    CU_ASSERT_PTR_NULL(non_directory_file_operands[2]);
    CU_ASSERT_STRING_EQUAL(directory_file_operands[0], "dir1");
    CU_ASSERT_STRING_EQUAL(directory_file_operands[1], "dirlink");
    CU_ASSERT_STRING_EQUAL(directory_file_operands[2], "empty");
    CU_ASSERT_PTR_NULL(directory_file_operands[3]);
    CU_ASSERT_FALSE(result_has_succeed(parsed_arguments_result));
    CU_ASSERT_PTR_NOT_NULL(sut);
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
        CU_add_test(suite, "should_return_NULL_non_directory_file_operands_if_NULL_parsed_arguments_are_passed", should_return_NULL_non_directory_file_operands_if_NULL_parsed_arguments_are_passed);
        CU_add_test(suite, "should_return_the_non_directory_file_operands", should_return_the_non_directory_file_operands);
        CU_add_test(suite, "should_return_NULL_directory_file_operands_if_NULL_parsed_arguments_are_passed", should_return_NULL_directory_file_operands_if_NULL_parsed_arguments_are_passed);
        CU_add_test(suite, "should_return_the_directory_file_operands", should_return_the_directory_file_operands);
        CU_add_test(suite, "should_return_false_for_has_any_option_if_NULL_parsed_arguments_are_passed", should_return_false_for_has_any_option_if_NULL_parsed_arguments_are_passed);
        CU_add_test(suite, "should_return_false_for_has_any_option_if_at_no_option_was_specified", should_return_false_for_has_any_option_if_at_no_option_was_specified);
        CU_add_test(suite, "should_return_true_for_has_any_option_if_at_least_one_option_was_specified", should_return_true_for_has_any_option_if_at_least_one_option_was_specified);
        CU_add_test(suite, "should_return_false_for_has_option_if_NULL_parsed_arguments_are_passed", should_return_false_for_has_option_if_NULL_parsed_arguments_are_passed);
        CU_add_test(suite, "should_return_false_for_has_option_if_the_specified_option_was_NOT_parsed", should_return_false_for_has_option_if_the_specified_option_was_NOT_parsed);
        CU_add_test(suite, "should_return_true_for_has_option_if_the_specified_option_was_parsed", should_return_true_for_has_option_if_the_specified_option_was_parsed);
        CU_add_test(suite, "should_return_false_for_multiple_file_operands_if_NULL_parsed_arguments_are_passed", should_return_false_for_multiple_file_operands_if_NULL_parsed_arguments_are_passed);
        CU_add_test(suite, "should_return_false_for_multiple_file_operands_if_has_less_than_two", should_return_false_for_multiple_file_operands_if_has_less_than_two);
        CU_add_test(suite, "should_return_true_for_multiple_file_operands_if_has_equal_or_more_than_two", should_return_true_for_multiple_file_operands_if_has_equal_or_more_than_two);
        CU_add_test(suite, "should_return_false_for_has_directory_file_operands_if_NULL_parsed_arguments_are_passed", should_return_false_for_has_directory_file_operands_if_NULL_parsed_arguments_are_passed);
        CU_add_test(suite, "should_return_false_for_has_directory_file_operands_if_it_does_not_have_any", should_return_false_for_has_directory_file_operands_if_it_does_not_have_any);
        CU_add_test(suite, "should_return_true_for_has_directory_file_operands_if_it_has_at_least_one", should_return_true_for_has_directory_file_operands_if_it_has_at_least_one);
        CU_add_test(suite, "should_return_false_for_has_mixed_types_file_operands_if_it_null_parsed_arguments_are_specified", should_return_false_for_has_mixed_types_file_operands_if_it_null_parsed_arguments_are_specified);
        CU_add_test(suite, "should_return_false_for_has_mixed_types_file_operands_if_it_does_not_have_at_least_one_non_dir_and_one_dir", should_return_false_for_has_mixed_types_file_operands_if_it_does_not_have_at_least_one_non_dir_and_one_dir);
        CU_add_test(suite, "should_return_true_for_has_mixed_types_file_operands_if_it_does_have_at_least_one_non_dir_and_one_dir", should_return_true_for_has_mixed_types_file_operands_if_it_does_have_at_least_one_non_dir_and_one_dir);
        CU_add_test(suite, "should_sort_the_non_directory_file_operands_by_ascii_by_default", should_sort_the_non_directory_file_operands_by_ascii_by_default);
        CU_add_test(suite, "should_sort_the_directory_file_operands_by_ascii_by_default", should_sort_the_directory_file_operands_by_ascii_by_default);
        CU_add_test(suite, "should_parse_the_recursive_option", should_parse_the_recursive_option);
        //------
        CU_add_test(suite, "should_be_created_correctly_even_if_current_directory_is_invalid_if_no_parameters_were_specified", should_be_created_correctly_even_if_current_directory_is_invalid_if_no_parameters_were_specified);
        CU_add_test(suite, "should_be_created_correctly_even_if_current_directory_is_invalid_and_was_specified", should_be_created_correctly_even_if_current_directory_is_invalid_and_was_specified);
        CU_add_test(suite, "should_be_created_correctly_even_if_the_specified_argument_cannot_be_accessed", should_be_created_correctly_even_if_the_specified_argument_cannot_be_accessed);
        CU_add_test(suite, "should_be_created_correctly_even_if_one_argument_cannot_be_accessed", should_be_created_correctly_even_if_one_argument_cannot_be_accessed);
        CU_add_test(suite, "should_be_created_correctly_even_if_one_argument_cannot_be_accessed_with_multiple_mixed_file_operands", should_be_created_correctly_even_if_one_argument_cannot_be_accessed_with_multiple_mixed_file_operands);
        CU_add_test(suite, "should_be_created_correctly_even_if_the_specified_arguments_cannot_be_accessed", should_be_created_correctly_even_if_the_specified_arguments_cannot_be_accessed);
        CU_add_test(suite, "should_be_created_correctly_even_if_some_arguments_cannot_be_accessed", should_be_created_correctly_even_if_some_arguments_cannot_be_accessed);
    }
}
