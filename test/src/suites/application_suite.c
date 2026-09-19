#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "mocks.h"
#include "application.h"

#define SUITE_NAME "application"

static t_result *parsed_arguments_result;
static t_parsed_arguments *parsed_arguments = NULL;

static void test_setup(void)
{
    reset_printing_buffer();
    vfs_mock_reset();
}

static void test_teardown(void)
{
    result_destroy(&parsed_arguments_result);
    parsed_arguments_destroy(&parsed_arguments);
}

static void get_parsed_arguments_result(const int argc, const char **args)
{
    parsed_arguments_result = parse_arguments(argc, args);
    parsed_arguments = result_get_value(parsed_arguments_result);
}

static void assert_application_execution_succeed(const int result)
{
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_SUCCESS);
}

static void should_return_a_major_error_when_passing_a_NULL_parsed_argument(void)
{
    CU_ASSERT_EQUAL(application_run(parsed_arguments_result), FT_LS_APPLICATION_MAJOR_ERROR);
    CU_ASSERT(verify_that_no_output_was_printed());
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_successfully_print_the_contents_of_the_current_directory_one_per_line_if_no_file_operands_are_specified(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file1", "subdir1", "symlink", "zz"),
        MOCK_FILE("./file1"),
        MOCK_DIR("./subdir1", ".", ".."),
        MOCK_SYMLINK("./symlink", "file1"),
        MOCK_FILE("./zz"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { NULL };
    const char *expected_file_names[] = { "file1", "subdir1", "symlink", "zz" };
    get_parsed_arguments_result(0, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is("%s\n%s\n%s\n%s\n",
        expected_file_names[0],
        expected_file_names[1],
        expected_file_names[2],
        expected_file_names[3]
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_file_name_if_a_regular_file_operand_is_specified(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("../../ft_ls/test/./frameworks/../regular_file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *file_name = "../../ft_ls/test/./frameworks/../regular_file";
    const char *arguments[] = { file_name, NULL };
    get_parsed_arguments_result(1, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is("%s\n", file_name));
    CU_ASSERT(verify_that_no_error_was_printed());
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
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dir", NULL };
    const char *expected_file_names[] = { "block_device", "char_device", "file_from_dir_1", "subdir_1" };
    get_parsed_arguments_result(1, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is("%s\n%s\n%s\n%s\n",
        expected_file_names[0],
        expected_file_names[1],
        expected_file_names[2],
        expected_file_names[3]
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_contents_of_multiple_non_directory_files(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_BLOCK_DEVICE("block_device"),
        MOCK_CHAR_DEVICE("cd1"),
        MOCK_FILE("file1"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "block_device", "cd1", "cd1", "file1", NULL };
    get_parsed_arguments_result(4, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n%s\n%s\n",
        arguments[0],
        arguments[1],
        arguments[2],
        arguments[3]
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
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
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dir", "dir2", "dir3", NULL };
    const char *expected_first_dir_file_name = "file_dir_1";
    const char *expected_second_dir_file_names[] = { "file_dir_2", "symlink" };
    const char *expected_third_dir_file_name = "file_dir_3";
    get_parsed_arguments_result(3, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
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
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_contents_of_the_mixed_types_specified_operands(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file"),
        MOCK_DIR("dir", ".", "..", "file", "dir2"),
        MOCK_SYMLINK("linkdir", "dir"),
        MOCK_SYMLINK("linklink", "linkdir"),
        MOCK_SYMLINK("linkfile", "file"),
        MOCK_BLOCK_DEVICE("block_device"),
        MOCK_CHAR_DEVICE("char_device"),
        MOCK_FIFO("fifo"),
        MOCK_SOCKET("socket"),
        MOCK_FILE("dir/file"),
        MOCK_DIR("dir/dir2", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "file", "dir", "linklink", "linkdir", "linkfile", "block_device", "char_device", "fifo", "socket", NULL };
    const char *expected_file_names[] = { "block_device", "char_device", "fifo", "file", "linkfile", "socket" };
    const char *expected_dir_file_names[] = { "dir2", "file" };
    get_parsed_arguments_result(9, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n%s\n%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n",
        expected_file_names[0],
        expected_file_names[1],
        expected_file_names[2],
        expected_file_names[3],
        expected_file_names[4],
        expected_file_names[5],
        arguments[1],
        expected_dir_file_names[0],
        expected_dir_file_names[1],
        arguments[3],
        expected_dir_file_names[0],
        expected_dir_file_names[1],
        arguments[2],
        expected_dir_file_names[0],
        expected_dir_file_names[1]
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_contents_of_the_explicitly_specified_hidden_non_directory_file_operands(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_CHAR_DEVICE(".char_device"),
        MOCK_FILE(".file1"),
        MOCK_SYMLINK("symlink", ".file1"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { ".char_device", ".file1", "symlink", NULL };
    get_parsed_arguments_result(3, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n%s\n",
        arguments[0],
        arguments[1],
        arguments[2]
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
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
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { ".dir", ".dir2", "dir1", NULL };
    const char *expected_first_dir_file_names[] = { "char_device", "file_from_dir_1" };
    const char *expected_second_dir_file_names[] = { "file_from_dir_2", "symlink" };
    const char *expected_third_dir_file_names[] = { "char_device", "file_from_dir_3", "normal_file" };
    get_parsed_arguments_result(3, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
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
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_contents_of_the_explicitly_specified_hidden_mixed_file_operands(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE(".file1"),
        MOCK_SYMLINK(".symlink", ".file1"),
        MOCK_DIR(".dir", ".run", ".", "char_device", "..", "file_from_dir_1"),
        MOCK_DIR(".dir1", "..", ".gitignore", "file_from_dir_2", ".", "symlink", ".vscode"),
        MOCK_DIR(".dir/.run", "..", "."),
        MOCK_CHAR_DEVICE(".dir/char_device"),
        MOCK_FILE(".dir/file_from_dir_1"),
        MOCK_DIR(".dir1/.vscode", "..", "."),
        MOCK_FILE(".dir1/.gitignore"),
        MOCK_FILE(".dir1/file_from_dir_2"),
        MOCK_SYMLINK(".dir1/symlink", ".dir1/file_from_dir_2"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { ".dir", ".dir1", ".file1", ".symlink", NULL };
    const char *expected_first_dir_file_names[] = { "char_device", "file_from_dir_1" };
    const char *expected_second_dir_file_names[] = { "file_from_dir_2", "symlink" };
    get_parsed_arguments_result(4, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
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
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_successfully_not_print_anything_if_the_specified_directory_is_empty(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dir", NULL };
    get_parsed_arguments_result(1, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_no_output_was_printed());
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_successfully_not_print_anything_if_the_specified_directory_only_has_hidden_files(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".dir", ".", ".gitignore", "..", ".idea/", ".run/"),
        MOCK_FILE(".dir/.gitignore"),
        MOCK_DIR(".dir/.idea", ".", ".."),
        MOCK_DIR(".dir/.run", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { ".dir", NULL };
    get_parsed_arguments_result(1, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_no_output_was_printed());
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_successfully_only_print_dir_headers_if_the_specified_directories_are_empty(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".dir", ".", ".."),
        MOCK_DIR("dir1", "..", "."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { ".dir", "dir1", NULL };
    get_parsed_arguments_result(2, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s:\n"
        "\n%s:\n",
        arguments[0],
        arguments[1]
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
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
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { ".dir1", "dir", NULL };
    get_parsed_arguments_result(2, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s:\n"
        "\n%s:\n",
        arguments[0],
        arguments[1]
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_successfully_only_print_dir_headers_with_non_directory_files_if_the_specified_directories_are_empty(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE(".hidden"),
        MOCK_FILE("file"),
        MOCK_DIR(".dir1", ".", ".."),
        MOCK_DIR("dir", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { ".hidden", ".dir1", "dir", "file", NULL };
    get_parsed_arguments_result(4, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "\n%s:\n",
        arguments[0],
        arguments[3],
        arguments[1],
        arguments[2]
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
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
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dir", ".hidden", "dir1", "file", NULL };
    get_parsed_arguments_result(4, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "\n%s:\n",
        arguments[1],
        arguments[3],
        arguments[0],
        arguments[2]
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
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
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { NULL };
    const char *expected_file_names[] = { "2file", "FILE", "_DIR", "_file", "a", "dir", "f" };
    get_parsed_arguments_result(0, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
        expected_file_names[0],
        expected_file_names[1],
        expected_file_names[2],
        expected_file_names[3],
        expected_file_names[4],
        expected_file_names[5],
        expected_file_names[6]
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
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
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dir", NULL };
    const char *expected_file_names[] = { "2file", "FILE", "_DIR", "_file", "a", "dir", "f" };
    get_parsed_arguments_result(1, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
        expected_file_names[0],
        expected_file_names[1],
        expected_file_names[2],
        expected_file_names[3],
        expected_file_names[4],
        expected_file_names[5],
        expected_file_names[6]
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
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
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "a", "2file", "_file", "f", ".hidden_file", "FILE", NULL };
    const char *expected_file_names[] = { ".hidden_file", "2file", "FILE", "_file", "a", "f" };
    get_parsed_arguments_result(6, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n%s\n%s\n%s\n%s\n",
        expected_file_names[0],
        expected_file_names[1],
        expected_file_names[2],
        expected_file_names[3],
        expected_file_names[4],
        expected_file_names[5]
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
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
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dir", "_dir", "DIR", NULL };
    const char *expected_first_dir_file_names[] = { "-file", "file1", "~file" };
    const char *expected_second_dir_file_names[] = { "FILE", "file2", "symlink" };
    const char *expected_third_dir_file_names[] = { "2FILE", "file3" };
    get_parsed_arguments_result(3, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
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
    CU_ASSERT(verify_that_no_error_was_printed());
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
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "a", "2file", "_DIR", ".hiddir", "_file", "f", "dir", ".hidden_file", "FILE", NULL };
    const char *expected_non_dir_file_names[] = { ".hidden_file", "2file", "FILE", "_file", "a", "f" };
    const char *expected_first_dir_file_names[] = { "-file", "file1", "~file" };
    const char *expected_second_dir_file_names[] = { "FILE", "file2", "symlink" };
    const char *expected_third_dir_file_names[] = { "2FILE", "file3" };
    get_parsed_arguments_result(9, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
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
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_contents_of_the_current_directory_without_following_symlinks(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "1_file", "2_subdir", "3_linkfile", "4_linkdir", "5_chardevice", "6_blockdevice", "7_fifo", "8_socket"),
        MOCK_FILE("./1_file"),
        MOCK_DIR("./2_subdir", ".", ".."),
        MOCK_SYMLINK("./3_linkfile", "1_file"),
        MOCK_SYMLINK("./4_linkdir", "2_subdir"),
        MOCK_CHAR_DEVICE("./5_chardevice"),
        MOCK_BLOCK_DEVICE("./6_blockdevice"),
        MOCK_FIFO("./7_fifo"),
        MOCK_SOCKET("./8_socket"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { NULL };
    const char *expected_file_names[] = { "1_file", "2_subdir", "3_linkfile", "4_linkdir", "5_chardevice", "6_blockdevice", "7_fifo", "8_socket" };
    get_parsed_arguments_result(0, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",
        expected_file_names[0],
        expected_file_names[1],
        expected_file_names[2],
        expected_file_names[3],
        expected_file_names[4],
        expected_file_names[5],
        expected_file_names[6],
        expected_file_names[7]
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_name_of_the_specified_symlink_pointing_to_a_non_directory_file(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file"),
        MOCK_SYMLINK("linkfile", "file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "linkfile", NULL };
    const char *expected_file_names[] = { "linkfile" };
    get_parsed_arguments_result(1, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is("%s\n",
        expected_file_names[0]
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_contents_of_the_directory_pointed_by_the_specified_symlink(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "..", "block_device", "char_device", "file", "subdir"),
        MOCK_SYMLINK("linkdir", "dir"),
        MOCK_BLOCK_DEVICE("dir/block_device"),
        MOCK_CHAR_DEVICE("dir/char_device"),
        MOCK_FILE("dir/file"),
        MOCK_DIR("dir/subdir", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "linkdir", NULL };
    const char *expected_file_names[] = { "block_device", "char_device", "file", "subdir" };
    get_parsed_arguments_result(1, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is("%s\n%s\n%s\n%s\n",
        expected_file_names[0],
        expected_file_names[1],
        expected_file_names[2],
        expected_file_names[3]
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_fail_with_a_major_error_and_not_print_anything_if_fails_to_open_the_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR_OPEN_ERROR(EACCES, "dir", ".", "..", "file1", "subdir1", "symlink", "zz"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dir", NULL };
    get_parsed_arguments_result(1, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_no_output_was_printed());
    CU_ASSERT(verify_that_the_error_printed_is("ft_ls: cannot open directory '%s': %s\n", "dir", strerror(EACCES)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_not_print_anything_if_fails_to_read_the_first_entry_of_the_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR_READ_ERROR("dir", 0, ".", "..", "file1", "subdir1", "symlink", "zz"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dir", NULL };
    get_parsed_arguments_result(1, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_no_output_was_printed());
    CU_ASSERT(verify_that_the_error_printed_is("ft_ls: reading directory '%s': %s\n", "dir", strerror(EBADF)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_only_print_the_non_failed_entries_if_fails_to_read_a_middle_entry_of_the_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR_READ_ERROR("dir", 3, ".", "..", "file1", "subdir1", "symlink", "zz"),
        MOCK_FILE("dir/file1"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dir", NULL };
    get_parsed_arguments_result(1, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is("file1\n"));
    CU_ASSERT(verify_that_the_error_printed_is("ft_ls: reading directory '%s': %s\n", "dir", strerror(EBADF)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_print_the_contents_of_the_directory_if_fails_to_close_it(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR_CLOSE_ERROR("dir", ".", "..", "file1", "subdir1", "symlink", "zz"),
        MOCK_FILE("dir/file1"),
        MOCK_DIR("dir/subdir1", ".", ".."),
        MOCK_SYMLINK("dir/symlink", "file1"),
        MOCK_FILE("dir/zz"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dir", NULL };
    const char *expected_file_names[] = { "file1", "subdir1", "symlink", "zz" };
    get_parsed_arguments_result(1, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is("%s\n%s\n%s\n%s\n",
        expected_file_names[0],
        expected_file_names[1],
        expected_file_names[2],
        expected_file_names[3]
    ));
    CU_ASSERT(verify_that_the_error_printed_is("ft_ls: closing directory '%s': %s\n", "dir", strerror(EBADF)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_print_only_the_non_directory_operand_if_fails_to_scan_the_directory_operand(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR_OPEN_ERROR(ENOTDIR, "dir", ".", "..", "file1", "subdir1", "symlink", "zz"),
        MOCK_FILE("file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dir", "file", NULL };
    get_parsed_arguments_result(2, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is("file\n\n"));
    CU_ASSERT(verify_that_the_error_printed_is("ft_ls: cannot open directory '%s': %s\n", "dir", strerror(ENOTDIR)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_print_only_the_working_directory_operand_if_fails_to_scan_a_directory_operand(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR_OPEN_ERROR(EACCES, "noPermDir", ".", "..", "file1", "subdir1", "symlink", "zz"),
        MOCK_DIR("workingDir", ".", "..", "fileDir"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *expected_file_name = "fileDir";
    const char *arguments[] = { "workingDir", "noPermDir", NULL };
    get_parsed_arguments_result(2, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s:\n"
        "%s\n",
        arguments[0],
        expected_file_name
    ));
    CU_ASSERT(verify_that_the_error_printed_is("ft_ls: cannot open directory '%s': %s\n", "noPermDir", strerror(EACCES)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_print_only_the_working_file_operands_if_fails_to_scan_a_directory_operand(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR_OPEN_ERROR(EACCES, "noPermDir", ".", "..", "file1", "subdir1", "symlink", "zz"),
        MOCK_FILE("file"),
        MOCK_FILE("zFile"),
        MOCK_DIR("workingDir", ".", "..", "fileDir"),
        MOCK_DIR("zWorkingDir", ".", "..", "zFileDir"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *expected_file_names[] = { "file", "zFile" };
    const char *expected_first_dir_file_name = "fileDir";
    const char *expected_second_dir_file_name = "zFileDir";
    const char *arguments[] = { "noPermDir", "workingDir", "file", "zWorkingDir", "zFile", NULL };
    get_parsed_arguments_result(5, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n"
        "\n%s:\n"
        "%s\n",
        expected_file_names[0],
        expected_file_names[1],
        arguments[1],
        expected_first_dir_file_name,
        arguments[3],
        expected_second_dir_file_name
    ));
    CU_ASSERT(verify_that_the_error_printed_is("ft_ls: cannot open directory '%s': %s\n", "noPermDir", strerror(EACCES)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_not_print_anything_if_all_directories_fail_to_scan(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR_OPEN_ERROR(EACCES, "dir", ".", "..", "file1", "subdir1", "symlink", "zz"),
        MOCK_DIR_READ_ERROR("zdir/", 4, ".", "..", "file1", "subdir1", "symlink", "zz"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "zdir/", "dir", NULL };
    const char *expected_file_names[] = { "file1", "subdir1" };
    get_parsed_arguments_result(2, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s:\n"
        "%s\n%s\n",
        arguments[0],
        expected_file_names[0],
        expected_file_names[1]
    ));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot open directory '%s': %s\n"
        "ft_ls: reading directory '%s': %s\n",
        "dir", strerror(EACCES),
        "zdir/", strerror(EBADF))
    );
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_print_only_the_working_file_operands_if_some_directories_fail_to_scan(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR_OPEN_ERROR(EACCES, "dir", ".", "..", "file1", "subdir1", "symlink", "zz"),
        MOCK_DIR_READ_ERROR("zdir/", 4, ".", "..", "file1", "subdir1", "symlink", "zz"),
        MOCK_FILE("file"),
        MOCK_FILE("zFile"),
        MOCK_DIR("workingDir", ".", "..", "fileDir"),
        MOCK_DIR("zWorkingDir", ".", "..", "zFileDir"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "zdir/", "file", "workingDir", "zWorkingDir", "zFile", "dir", NULL };
    const char *expected_file_names[] = { "file", "zFile" };
    const char *expected_first_dir_file_name = "fileDir";
    const char *expected_second_dir_file_name = "zFileDir";
    const char *expected_failed_dir_file_names[] = { "file1", "subdir1" };
    get_parsed_arguments_result(6, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n"
        "\n%s:\n"
        "%s\n"
        "\n%s:\n"
        "%s\n%s\n",
        expected_file_names[0],
        expected_file_names[1],
        arguments[2],
        expected_first_dir_file_name,
        arguments[3],
        expected_second_dir_file_name,
        arguments[0],
        expected_failed_dir_file_names[0],
        expected_failed_dir_file_names[1]
    ));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot open directory '%s': %s\n"
        "ft_ls: reading directory '%s': %s\n",
        "dir", strerror(EACCES),
        "zdir/", strerror(EBADF))
    );
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_not_print_anything_if_the_specified_file_operand_fails_to_be_accessed(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_ACCESS_ERROR(ENOTDIR, "notADirectory/"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "notADirectory/", NULL };
    get_parsed_arguments_result(1, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_no_output_was_printed());
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n",
        "notADirectory/", strerror(ENOTDIR))
    );
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_print_the_working_operand_if_one_file_operand_fails_to_be_accessed(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_ACCESS_ERROR(EACCES, "no/perms"),
        MOCK_DIR("dir", ".", "..", "file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "no/perms", "dir", NULL };
    get_parsed_arguments_result(2, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s:\n%s\n",
        "dir", "file"
    ));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n",
        "no/perms", strerror(EACCES))
    );
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_print_only_the_working_operands_if_one_file_operand_fails_to_be_accessed(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_ACCESS_ERROR(ELOOP, "linkloop"),
        MOCK_FILE("file"),
        MOCK_FILE("zFile"),
        MOCK_DIR("workingDir", ".", "..", "fileDir"),
        MOCK_DIR("zWorkingDir", ".", "..", "zFileDir"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "linkloop", "file", "workingDir", "zWorkingDir", "zFile", NULL };
    const char *expected_file_names[] = { "file", "zFile" };
    const char *expected_first_dir_file_name = "fileDir";
    const char *expected_second_dir_file_name = "zFileDir";
    get_parsed_arguments_result(6, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n"
        "\n%s:\n"
        "%s\n",
        expected_file_names[0],
        expected_file_names[1],
        arguments[2],
        expected_first_dir_file_name,
        arguments[3],
        expected_second_dir_file_name
    ));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n",
        "linkloop", strerror(ELOOP)
    ));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_not_print_anything_if_the_specified_file_operands_fail_to_be_accessed(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_ACCESS_ERROR(ENOTDIR, "notADirectory/"),
        MOCK_FILE_ACCESS_ERROR(ENAMETOOLONG, "nameTooLong"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "notADirectory/", "nameTooLong", NULL };
    get_parsed_arguments_result(2, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_no_output_was_printed());
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot access '%s': %s\n",
        "notADirectory/", strerror(ENOTDIR),
        "nameTooLong", strerror(ENAMETOOLONG)
    ));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_print_only_the_working_operands_if_some_file_operands_fail_to_be_accessed(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_ACCESS_ERROR(EACCES, "noPerms"),
        MOCK_FILE_ACCESS_ERROR(ENOENT, "nonExistent"),
        MOCK_FILE_ACCESS_ERROR(ENOTDIR, "notADir/"),
        MOCK_FILE("file"),
        MOCK_FILE("zFile"),
        MOCK_DIR("workingDir", ".", "..", "fileDir"),
        MOCK_DIR("zWorkingDir", ".", "..", "zFileDir"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "noPerms", "file", "workingDir", "zWorkingDir", "nonExistent", "zFile", "notADir/", NULL };
    const char *expected_file_names[] = { "file", "zFile" };
    const char *expected_first_dir_file_name = "fileDir";
    const char *expected_second_dir_file_name = "zFileDir";
    get_parsed_arguments_result(7, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n"
        "\n%s:\n"
        "%s\n",
        expected_file_names[0],
        expected_file_names[1],
        arguments[2],
        expected_first_dir_file_name,
        arguments[3],
        expected_second_dir_file_name
    ));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot access '%s': %s\n",
        "noPerms", strerror(EACCES),
        "nonExistent", strerror(ENOENT),
        "notADir/", strerror(ENOTDIR)
    ));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_not_print_anything_if_fails_to_access_an_operand_and_process_a_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_ACCESS_ERROR(ENOTDIR, "notADirectory/"),
        MOCK_DIR_OPEN_ERROR(EACCES, "noPermDir", ".", "..", "file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "noPermDir", "notADirectory/", NULL };
    get_parsed_arguments_result(2, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_no_output_was_printed());
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot open directory '%s': %s\n",
        "notADirectory/", strerror(ENOTDIR),
        "noPermDir", strerror(EACCES)
    ));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

void register_application_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, test_teardown);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_return_a_major_error_when_passing_a_NULL_parsed_argument", should_return_a_major_error_when_passing_a_NULL_parsed_argument);
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
        CU_add_test(suite, "should_successfully_print_the_contents_of_the_current_directory_without_following_symlinks", should_successfully_print_the_contents_of_the_current_directory_without_following_symlinks);
        CU_add_test(suite, "should_successfully_print_the_name_of_the_specified_symlink_pointing_to_a_non_directory_file", should_successfully_print_the_name_of_the_specified_symlink_pointing_to_a_non_directory_file);
        CU_add_test(suite, "should_successfully_print_the_contents_of_the_directory_pointed_by_the_specified_symlink", should_successfully_print_the_contents_of_the_directory_pointed_by_the_specified_symlink);
        CU_add_test(suite, "should_fail_with_a_major_error_and_not_print_anything_if_fails_to_open_the_directory", should_fail_with_a_major_error_and_not_print_anything_if_fails_to_open_the_directory);
        CU_add_test(suite, "should_fail_with_a_major_error_and_not_print_anything_if_fails_to_read_the_first_entry_of_the_directory", should_fail_with_a_major_error_and_not_print_anything_if_fails_to_read_the_first_entry_of_the_directory);
        CU_add_test(suite, "should_fail_with_a_major_error_and_only_print_the_non_failed_entries_if_fails_to_read_a_middle_entry_of_the_directory", should_fail_with_a_major_error_and_only_print_the_non_failed_entries_if_fails_to_read_a_middle_entry_of_the_directory);
        CU_add_test(suite, "should_fail_with_a_major_error_print_the_contents_of_the_directory_if_fails_to_close_it", should_fail_with_a_major_error_print_the_contents_of_the_directory_if_fails_to_close_it);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_only_the_non_directory_operand_if_fails_to_scan_the_directory_operand", should_fail_with_a_major_error_and_print_only_the_non_directory_operand_if_fails_to_scan_the_directory_operand);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_only_the_working_directory_operand_if_fails_to_scan_a_directory_operand", should_fail_with_a_major_error_and_print_only_the_working_directory_operand_if_fails_to_scan_a_directory_operand);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_only_the_working_file_operands_if_fails_to_scan_a_directory_operand", should_fail_with_a_major_error_and_print_only_the_working_file_operands_if_fails_to_scan_a_directory_operand);
        CU_add_test(suite, "should_fail_with_a_major_error_and_not_print_anything_if_all_directories_fail_to_scan", should_fail_with_a_major_error_and_not_print_anything_if_all_directories_fail_to_scan);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_only_the_working_file_operands_if_some_directories_fail_to_scan", should_fail_with_a_major_error_and_print_only_the_working_file_operands_if_some_directories_fail_to_scan);
        CU_add_test(suite, "should_fail_with_a_major_error_and_not_print_anything_if_the_specified_file_operand_fails_to_be_accessed", should_fail_with_a_major_error_and_not_print_anything_if_the_specified_file_operand_fails_to_be_accessed);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_the_working_operand_if_one_file_operand_fails_to_be_accessed", should_fail_with_a_major_error_and_print_the_working_operand_if_one_file_operand_fails_to_be_accessed);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_only_the_working_operands_if_one_file_operand_fails_to_be_accessed", should_fail_with_a_major_error_and_print_only_the_working_operands_if_one_file_operand_fails_to_be_accessed);
        CU_add_test(suite, "should_fail_with_a_major_error_and_not_print_anything_if_the_specified_file_operands_fail_to_be_accessed", should_fail_with_a_major_error_and_not_print_anything_if_the_specified_file_operands_fail_to_be_accessed);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_only_the_working_operands_if_some_file_operands_fail_to_be_accessed", should_fail_with_a_major_error_and_print_only_the_working_operands_if_some_file_operands_fail_to_be_accessed);
        CU_add_test(suite, "should_fail_with_a_major_error_and_not_print_anything_if_fails_to_access_an_operand_and_process_a_directory", should_fail_with_a_major_error_and_not_print_anything_if_fails_to_access_an_operand_and_process_a_directory);
    }
}
