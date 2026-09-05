#include <sys/stat.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "mocks.h"
#include "application.h"
#include "error_codes.h"

#define SUITE_NAME "application"

static t_parsed_arguments *parsed_arguments = NULL;

static void test_setup(void)
{
    reset_dirent_guarantees();
    reset_stat_guarantees();
    reset_printing_buffer();
    vfs_mock_reset();
}

static void test_teardown(void)
{
    parsed_arguments_destroy(&parsed_arguments);
}

static void assert_application_execution_succeed(const int result)
{
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_SUCCESS);
}

static void should_successfully_print_the_contents_of_the_current_directory_one_per_line_if_no_file_operands_are_specified(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file1", "subdir1", "symlink", "zz"),
        MOCK_FILE("./file1"),
        MOCK_DIR("./subdir1", ".", ".."),
        MOCK_SYMLINK("./symlink", "file1"),
        MOCK_FILE("./zz"),
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { NULL };
    const char *expected_file_names[] = { "file1", "subdir1", "symlink", "zz" };
    parsed_arguments = parse_arguments(0, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is("%s\n%s\n%s\n%s\n",
        expected_file_names[0],
        expected_file_names[1],
        expected_file_names[2],
        expected_file_names[3]
    ));
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_file_name_if_a_regular_file_operand_is_specified(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("../../ft_ls/test/./frameworks/../regular_file"),
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *file_name = "../../ft_ls/test/./frameworks/../regular_file";
    const char *arguments[] = { file_name, NULL };
    parsed_arguments = parse_arguments(1, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is("%s\n", file_name));
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_contents_of_the_directory_specified_as_an_operand(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "..", "block_device", "char_device", "file_from_dir_1", "subdir_1"),
        MOCK_BLOCK_DEVICE("dir/block_device"),
        MOCK_CHAR_DEVICE("dir/char_device"),
        MOCK_FILE("dir/file_from_dir_1"),
        MOCK_DIR("dir/subdir_1", ".", ".."),
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dir", NULL };
    const char *expected_file_names[] = { "block_device", "char_device", "file_from_dir_1", "subdir_1" };
    parsed_arguments = parse_arguments(1, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is("%s\n%s\n%s\n%s\n",
        expected_file_names[0],
        expected_file_names[1],
        expected_file_names[2],
        expected_file_names[3]
    ));
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_contents_of_multiple_non_directory_files(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_BLOCK_DEVICE("block_device"),
        MOCK_CHAR_DEVICE("cd1"),
        MOCK_FILE("file1"),
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "block_device", "cd1", "cd1", "file1", NULL };
    parsed_arguments = parse_arguments(4, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(
        "%s\n%s\n%s\n%s\n",
        arguments[0],
        arguments[1],
        arguments[2],
        arguments[3]
    ));
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_contents_of_multiple_directory_files(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "file_dir_1", ".."),
        MOCK_DIR("dir2", "file_dir_2", "..", ".", "symlink"),
        MOCK_DIR("dir3", "..", ".", "file_dir_3"),
        MOCK_FILE("dir/file_dir_1"),
        MOCK_FILE("dir2/file_dir_2"),
        MOCK_SYMLINK("dir2/symlink", "dir2/file_dir_2"),
        MOCK_FILE("dir3/file_dir_3"),
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dir", "dir2", "dir3", NULL };
    const char *expected_first_dir_file_name = "file_dir_1";
    const char *expected_second_dir_file_names[] = { "file_dir_2", "symlink" };
    const char *expected_third_dir_file_name = "file_dir_3";
    parsed_arguments = parse_arguments(3, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(
        "%s:\n"
        "%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n",
        arguments[0],
        expected_first_dir_file_name,
        arguments[1],
        expected_second_dir_file_names[0],
        expected_second_dir_file_names[1],
        arguments[2],
        expected_third_dir_file_name
    ));
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_contents_of_the_mixed_types_specified_operands(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file1"),
        MOCK_SYMLINK("symlink", "file1"),
        MOCK_DIR("dir", ".", "block_device", "char_device", "file_from_dir_1", "..", "subdir_1"),
        MOCK_BLOCK_DEVICE("dir/block_device"),
        MOCK_CHAR_DEVICE("dir/char_device"),
        MOCK_FILE("dir/file_from_dir_1"),
        MOCK_DIR("dir/subdir_1", ".", ".."),
        MOCK_DIR("dir1", "..", "file2", ".", "file_from_dir_2", "symlink"),
        MOCK_FILE("dir1/file2"),
        MOCK_FILE("dir1/file_from_dir_2"),
        MOCK_SYMLINK("dir1/symlink", "dir1/file_from_dir_2"),
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dir", "file1", "file1", "symlink", "dir1", NULL };
    const char *expected_first_dir_file_names[] = { "block_device", "char_device", "file_from_dir_1", "subdir_1" };
    const char *expected_second_dir_file_names[] = { "file2", "file_from_dir_2", "symlink" };
    parsed_arguments = parse_arguments(5, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(
        "%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n%s\n",
        arguments[1],
        arguments[2],
        arguments[3],
        arguments[0],
        expected_first_dir_file_names[0],
        expected_first_dir_file_names[1],
        expected_first_dir_file_names[2],
        expected_first_dir_file_names[3],
        arguments[4],
        expected_second_dir_file_names[0],
        expected_second_dir_file_names[1],
        expected_second_dir_file_names[2]
    ));
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_contents_of_the_explicitly_specified_hidden_non_directory_file_operands(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_CHAR_DEVICE(".char_device"),
        MOCK_FILE(".file1"),
        MOCK_SYMLINK("symlink", ".file1"),
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { ".char_device", ".file1", "symlink", NULL };
    parsed_arguments = parse_arguments(3, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(
        "%s\n%s\n%s\n",
        arguments[0],
        arguments[1],
        arguments[2]
    ));
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_contents_of_the_explicitly_specified_hidden_directory_file_operands(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".dir", ".", "char_device", "..", "file_from_dir_1", ".gitignore"),
        MOCK_DIR(".dir2", "..", "file_from_dir_2", ".", "symlink", ".idea"),
        MOCK_DIR("dir1", "char_device", ".", "file_from_dir_3", ".run", "..", "normal_file"),
        MOCK_CHAR_DEVICE(".dir/char_device"),
        MOCK_FILE(".dir/file_from_dir_1"),
        MOCK_FILE(".dir/.gitignore"),
        MOCK_FILE(".dir2/file_from_dir_2"),
        MOCK_SYMLINK(".dir2/symlink", "dir2/file_from_dir_2"),
        MOCK_DIR(".dir2/.idea", ".", ".."),
        MOCK_CHAR_DEVICE("dir1/char_device"),
        MOCK_FILE("dir1/file_from_dir_3"),
        MOCK_FILE("dir1/normal_file"),
        MOCK_DIR("dir1/.run", ".", ".."),
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { ".dir", ".dir2", "dir1", NULL };
    const char *expected_first_dir_file_names[] = { "char_device", "file_from_dir_1" };
    const char *expected_second_dir_file_names[] = { "file_from_dir_2", "symlink" };
    const char *expected_third_dir_file_names[] = { "char_device", "file_from_dir_3", "normal_file" };
    parsed_arguments = parse_arguments(3, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(
        "%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n%s\n",
        arguments[0],
        expected_first_dir_file_names[0],
        expected_first_dir_file_names[1],
        arguments[1],
        expected_second_dir_file_names[0],
        expected_second_dir_file_names[1],
        arguments[2],
        expected_third_dir_file_names[0],
        expected_third_dir_file_names[1],
        expected_third_dir_file_names[2]
    ));
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_contents_of_the_explicitly_specified_hidden_mixed_file_operands(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE(".file1"),
        MOCK_SYMLINK(".symlink", "file1"),
        MOCK_DIR(".dir", ".run", ".", "char_device", "..", "file_from_dir_1"),
        MOCK_DIR(".dir1", "..", ".gitignore", "file_from_dir_2", ".", "symlink", ".vscode"),
        MOCK_DIR(".dir/.run", "..", "."),
        MOCK_CHAR_DEVICE(".dir/char_device"),
        MOCK_FILE(".dir/file_from_dir_1"),
        MOCK_DIR(".dir1/.vscode", "..", "."),
        MOCK_FILE(".dir1/.gitignore"),
        MOCK_FILE(".dir1/file_from_dir_2"),
        MOCK_SYMLINK(".dir1/symlink", ".dir1/file_from_dir_2"),
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { ".dir", ".dir1", ".file1", ".symlink", NULL };
    const char *expected_first_dir_file_names[] = { "char_device", "file_from_dir_1" };
    const char *expected_second_dir_file_names[] = { "file_from_dir_2", "symlink" };
    parsed_arguments = parse_arguments(5, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n",
        arguments[2],
        arguments[3],
        arguments[0],
        expected_first_dir_file_names[0],
        expected_first_dir_file_names[1],
        arguments[1],
        expected_second_dir_file_names[0],
        expected_second_dir_file_names[1]
    ));
    assert_application_execution_succeed(result);
}

static void should_successfully_not_print_anything_if_the_specified_directory_is_empty(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", ".."),
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dir", NULL };
    parsed_arguments = parse_arguments(1, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(""));
    assert_application_execution_succeed(result);
}

static void should_successfully_not_print_anything_if_the_specified_directory_only_has_hidden_files(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".dir", ".", ".gitignore", "..", ".idea/", ".run/"),
        MOCK_FILE(".dir/.gitignore"),
        MOCK_DIR(".dir/.idea", ".", ".."),
        MOCK_DIR(".dir/.run", ".", ".."),
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { ".dir", NULL };
    parsed_arguments = parse_arguments(1, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(""));
    assert_application_execution_succeed(result);
}

static void should_successfully_only_print_dir_headers_if_the_specified_directories_are_empty(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".dir", ".", ".."),
        MOCK_DIR("dir1", "..", "."),
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { ".dir", "dir1", NULL };
    parsed_arguments = parse_arguments(2, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(
        "%s:\n"
        "\n%s:\n",
        arguments[0],
        arguments[1]
    ));
    assert_application_execution_succeed(result);
}

static void should_successfully_only_print_dir_headers_if_the_specified_directories_only_have_hidden_files(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".dir1", "..", ".gitignore", ".idea", "."),
        MOCK_DIR("dir", ".", ".run", ".."),
        MOCK_FILE(".dir1/.gitignore"),
        MOCK_DIR(".dir1/.idea", ".", ".."),
        MOCK_DIR("dir/.run", ".", ".."),
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { ".dir1", "dir", NULL };
    parsed_arguments = parse_arguments(2, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(
        "%s:\n"
        "\n%s:\n",
        arguments[0],
        arguments[1]
    ));
    assert_application_execution_succeed(result);
}

static void should_successfully_only_print_dir_headers_with_non_directory_files_if_the_specified_directories_are_empty(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE(".hidden"),
        MOCK_FILE("file"),
        MOCK_DIR(".dir1", ".", ".."),
        MOCK_DIR("dir", ".", ".."),
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { ".hidden", ".dir1", "dir", "file", NULL };
    parsed_arguments = parse_arguments(4, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "\n%s:\n",
        arguments[0],
        arguments[3],
        arguments[1],
        arguments[2]
    ));
    assert_application_execution_succeed(result);
}

static void should_successfully_only_print_dir_headers_with_non_directory_files_if_the_specified_directories_only_have_hidden_files(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE(".hidden"),
        MOCK_FILE("file"),
        MOCK_DIR("dir", "..", ".gitignore", ".idea", "."),
        MOCK_DIR("dir1", ".", ".run", ".."),
        MOCK_FILE("dir/.gitignore"),
        MOCK_DIR("dir/.idea", ".", ".."),
        MOCK_DIR("dir1/.run", ".", ".."),
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dir", ".hidden", "dir1", "file", NULL };
    parsed_arguments = parse_arguments(4, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "\n%s:\n",
        arguments[1],
        arguments[3],
        arguments[0],
        arguments[2]
    ));
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_contents_of_the_current_directory_sorted_if_no_file_operands_are_specified(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", "a", "2file", "..", "_DIR", ".hiddir", "_file", "f", "dir", ".hidden_file", ".", "FILE"),
        MOCK_FILE("./a"),
        MOCK_FILE("./2file"),
        MOCK_DIR("./_DIR", ".", ".."),
        MOCK_DIR("./.hiddir", ".", ".."),
        MOCK_FILE("./_file"),
        MOCK_FILE("./f"),
        MOCK_DIR("./dir", ".", ".."),
        MOCK_FILE("./.hidden_file"),
        MOCK_FILE("./FILE"),
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { NULL };
    const char *expected_file_names[] = { "2file", "FILE", "_DIR", "_file", "a", "dir", "f" };
    parsed_arguments = parse_arguments(0, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(
        "%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
        expected_file_names[0],
        expected_file_names[1],
        expected_file_names[2],
        expected_file_names[3],
        expected_file_names[4],
        expected_file_names[5],
        expected_file_names[6]
    ));
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_contents_of_the_specified_directory_sorted(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", "a", "2file", "..", "_DIR", ".hiddir", "_file", "f", "dir", ".hidden_file", ".", "FILE"),
        MOCK_FILE("dir/a"),
        MOCK_FILE("dir/2file"),
        MOCK_DIR("dir/_DIR", ".", ".."),
        MOCK_DIR("dir/.hiddir", ".", ".."),
        MOCK_FILE("dir/_file"),
        MOCK_FILE("dir/f"),
        MOCK_DIR("dir/dir", ".", ".."),
        MOCK_FILE("dir/.hidden_file"),
        MOCK_FILE("dir/FILE"),
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dir", NULL };
    const char *expected_file_names[] = { "2file", "FILE", "_DIR", "_file", "a", "dir", "f" };
    parsed_arguments = parse_arguments(1, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(
        "%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
        expected_file_names[0],
        expected_file_names[1],
        expected_file_names[2],
        expected_file_names[3],
        expected_file_names[4],
        expected_file_names[5],
        expected_file_names[6]
    ));
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_specified_non_directory_file_operands_sorted(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("a"),
        MOCK_FILE("2file"),
        MOCK_FILE("_file"),
        MOCK_FILE("f"),
        MOCK_FILE(".hidden_file"),
        MOCK_FILE("FILE"),
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "a", "2file", "_file", "f", ".hidden_file", "FILE", NULL };
    const char *expected_file_names[] = { ".hidden_file", "2file", "FILE", "_file", "a", "f" };
    parsed_arguments = parse_arguments(6, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(
        "%s\n%s\n%s\n%s\n%s\n%s\n",
        expected_file_names[0],
        expected_file_names[1],
        expected_file_names[2],
        expected_file_names[3],
        expected_file_names[4],
        expected_file_names[5]
    ));
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_specified_directory_file_operands_and_their_contents_sorted(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", "~file", ".", "file1", "..", "-file"),
        MOCK_DIR("_dir", "file2", "..", "FILE", ".", "symlink"),
        MOCK_DIR("DIR", "..", "2FILE", ".", "file3"),
        MOCK_FILE("dir/~file"),
        MOCK_FILE("dir/file1"),
        MOCK_FILE("dir/-file"),
        MOCK_FILE("_dir/file2"),
        MOCK_FILE("_dir/FILE"),
        MOCK_SYMLINK("_dir/symlink", "_dir/FILE"),
        MOCK_FILE("DIR/2FILE"),
        MOCK_FILE("DIR/file3"),
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dir", "_dir", "DIR", NULL };
    const char *expected_first_dir_file_names[] = { "-file", "file1", "~file" };
    const char *expected_second_dir_file_names[] = { "FILE", "file2", "symlink" };
    const char *expected_third_dir_file_names[] = { "2FILE", "file3" };
    parsed_arguments = parse_arguments(3, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(
        "%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n%s\n",
        arguments[2],
        expected_third_dir_file_names[0],
        expected_third_dir_file_names[1],
        arguments[1],
        expected_second_dir_file_names[0],
        expected_second_dir_file_names[1],
        expected_second_dir_file_names[2],
        arguments[0],
        expected_first_dir_file_names[0],
        expected_first_dir_file_names[1],
        expected_first_dir_file_names[2]
    ));
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_specified_mixed_types_file_operands_and_their_contents_sorted(void)
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
        { .path = NULL, .mode = 0, .entries = NULL, .target = NULL }
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "a", "2file", "_DIR", ".hiddir", "_file", "f", "dir", ".hidden_file", "FILE", NULL };
    const char *expected_non_dir_file_names[] = { ".hidden_file", "2file", "FILE", "_file", "a", "f" };
    const char *expected_first_dir_file_names[] = { "-file", "file1", "~file" };
    const char *expected_second_dir_file_names[] = { "FILE", "file2", "symlink" };
    const char *expected_third_dir_file_names[] = { "2FILE", "file3" };
    parsed_arguments = parse_arguments(9, arguments);

    const int result = application_run(parsed_arguments);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(
        "%s\n%s\n%s\n%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n",
        expected_non_dir_file_names[0],
        expected_non_dir_file_names[1],
        expected_non_dir_file_names[2],
        expected_non_dir_file_names[3],
        expected_non_dir_file_names[4],
        expected_non_dir_file_names[5],
        arguments[3],
        expected_second_dir_file_names[0],
        expected_second_dir_file_names[1],
        expected_second_dir_file_names[2],
        arguments[2],
        expected_first_dir_file_names[0],
        expected_first_dir_file_names[1],
        expected_first_dir_file_names[2],
        arguments[6],
        expected_third_dir_file_names[0],
        expected_third_dir_file_names[1]
    ));
    assert_application_execution_succeed(result);
}

void register_application_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, test_teardown);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_successfully_print_the_contents_of_the_current_directory_one_per_line_if_no_file_operands_are_specified", should_successfully_print_the_contents_of_the_current_directory_one_per_line_if_no_file_operands_are_specified);
        CU_add_test(suite, "should_successfully_print_the_file_name_if_a_regular_file_operand_is_specified", should_successfully_print_the_file_name_if_a_regular_file_operand_is_specified);
        CU_add_test(suite, "should_successfully_print_the_contents_of_the_directory_specified_as_an_operand", should_successfully_print_the_contents_of_the_directory_specified_as_an_operand);
        CU_add_test(suite, "should_successfully_print_the_contents_of_multiple_non_directory_files", should_successfully_print_the_contents_of_multiple_non_directory_files);
        CU_add_test(suite, "should_successfully_print_the_contents_of_multiple_directory_files", should_successfully_print_the_contents_of_multiple_directory_files);
        CU_add_test(suite, "should_successfully_print_the_contents_of_the_mixed_types_specified_operands", should_successfully_print_the_contents_of_the_mixed_types_specified_operands);
        CU_add_test(suite, "should_successfully_print_the_contents_of_the_explicitly_specified_hidden_non_directory_file_operands", should_successfully_print_the_contents_of_the_explicitly_specified_hidden_non_directory_file_operands);
        CU_add_test(suite, "should_successfully_print_the_contents_of_the_explicitly_specified_hidden_directory_file_operands", should_successfully_print_the_contents_of_the_explicitly_specified_hidden_directory_file_operands);
        CU_add_test(suite, "should_successfully_print_the_contents_of_the_explicitly_specified_hidden_mixed_file_operands", should_successfully_print_the_contents_of_the_explicitly_specified_hidden_mixed_file_operands);
        CU_add_test(suite, "should_successfully_not_print_anything_if_the_specified_directory_is_empty", should_successfully_not_print_anything_if_the_specified_directory_is_empty);
        CU_add_test(suite, "should_successfully_not_print_anything_if_the_specified_directory_only_has_hidden_files", should_successfully_not_print_anything_if_the_specified_directory_only_has_hidden_files);
        CU_add_test(suite, "should_successfully_only_print_dir_headers_if_the_specified_directories_are_empty", should_successfully_only_print_dir_headers_if_the_specified_directories_are_empty);
        CU_add_test(suite, "should_successfully_only_print_dir_headers_if_the_specified_directories_only_have_hidden_files", should_successfully_only_print_dir_headers_if_the_specified_directories_only_have_hidden_files);
        CU_add_test(suite, "should_successfully_only_print_dir_headers_with_non_directory_files_if_the_specified_directories_are_empty", should_successfully_only_print_dir_headers_with_non_directory_files_if_the_specified_directories_are_empty);
        CU_add_test(suite, "should_successfully_only_print_dir_headers_with_non_directory_files_if_the_specified_directories_only_have_hidden_files", should_successfully_only_print_dir_headers_with_non_directory_files_if_the_specified_directories_only_have_hidden_files);
        CU_add_test(suite, "should_successfully_print_the_contents_of_the_current_directory_sorted_if_no_file_operands_are_specified", should_successfully_print_the_contents_of_the_current_directory_sorted_if_no_file_operands_are_specified);
        CU_add_test(suite, "should_successfully_print_the_contents_of_the_specified_directory_sorted", should_successfully_print_the_contents_of_the_specified_directory_sorted);
        CU_add_test(suite, "should_successfully_print_the_specified_non_directory_file_operands_sorted", should_successfully_print_the_specified_non_directory_file_operands_sorted);
        CU_add_test(suite, "should_successfully_print_the_specified_directory_file_operands_and_their_contents_sorted", should_successfully_print_the_specified_directory_file_operands_and_their_contents_sorted);
        CU_add_test(suite, "should_successfully_print_the_specified_mixed_types_file_operands_and_their_contents_sorted", should_successfully_print_the_specified_mixed_types_file_operands_and_their_contents_sorted);
    }
}
