#include "suites.h"
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

static void assert_application_succeeded_with_output(const int result, const char *expected_output)
{
    CU_ASSERT(verify_that_the_output_printed_is("%s", expected_output));
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
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
        MOCK_FILE("file1"),
        MOCK_DIR("subdir1", ".", ".."),
        MOCK_SYMLINK("symlink", "file1"),
        MOCK_FILE("zz"),
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

static void should_successfully_print_the_contents_of_the_current_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file1", "subdir1", "symlink", "zz"),
        MOCK_FILE("file1"),
        MOCK_DIR("subdir1", ".", ".."),
        MOCK_SYMLINK("symlink", "file1"),
        MOCK_FILE("zz"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { ".", NULL };
    const char *expected_file_names[] = { "file1", "subdir1", "symlink", "zz" };
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

static void should_successfully_print_directory_contents_in_reverse_order_when_reverse_option_is_active(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "..", "alpha", "zeta"),
        MOCK_FILE("dir/alpha"),
        MOCK_FILE("dir/zeta"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-r", "dir", NULL };
    get_parsed_arguments_result(2, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is("zeta\nalpha\n"));
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
        MOCK_FILE("linkdir/file"),
        MOCK_DIR("linkdir/dir2", ".", ".."),
        MOCK_FILE("linklink/file"),
        MOCK_DIR("linklink/dir2", ".", ".."),
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
        MOCK_FILE("a"),
        MOCK_FILE("2file"),
        MOCK_DIR("_DIR", ".", ".."),
        MOCK_DIR(".hiddir", ".", ".."),
        MOCK_FILE("_file"),
        MOCK_FILE("f"),
        MOCK_DIR("dir", ".", ".."),
        MOCK_FILE(".hidden_file"),
        MOCK_FILE("FILE"),
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
        MOCK_FILE("1_file"),
        MOCK_DIR("2_subdir", ".", ".."),
        MOCK_SYMLINK("3_linkfile", "1_file"),
        MOCK_SYMLINK("4_linkdir", "2_subdir"),
        MOCK_CHAR_DEVICE("5_chardevice"),
        MOCK_BLOCK_DEVICE("6_blockdevice"),
        MOCK_FIFO("7_fifo"),
        MOCK_SOCKET("8_socket"),
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
        MOCK_BLOCK_DEVICE("linkdir/block_device"),
        MOCK_CHAR_DEVICE("linkdir/char_device"),
        MOCK_FILE("linkdir/file"),
        MOCK_DIR("linkdir/subdir", ".", ".."),
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

static void should_successfully_print_the_contents_of_the_symlinks_to_different_file_types(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file"),
        MOCK_DIR("dir", ".", "..", "dirFile", "symlink", "symDir"),
        MOCK_SYMLINK("fileLink", "file"),
        MOCK_SYMLINK("dirLink", "dir"),
        MOCK_SYMLINK("multihopFile", "fileLink"),
        MOCK_SYMLINK("multihopDir", "dirLink"),
        MOCK_SYMLINK("dir/symlink", "dir/dirFile"),
        MOCK_SYMLINK("dir/symDir", "dir"),
        MOCK_BROKEN_LINK("brokenLink"),
        MOCK_LOOP_LINK("loopLink"),
        MOCK_FILE_ACCESS_ERROR(ENOENT, "nonExistent"),
        MOCK_FILE("dir/dirFile"),
        MOCK_DIR("dir/symDir", ".", ".."),
        MOCK_FILE("dirLink/dirFile"),
        MOCK_SYMLINK("dirLink/symlink", "dirLink/dirFile"),
        MOCK_DIR("dirLink/symDir", ".", ".."),
        MOCK_FILE("multihopDir/dirFile"),
        MOCK_SYMLINK("multihopDir/symlink", "multihopDir/dirFile"),
        MOCK_DIR("multihopDir/symDir", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dirLink", "fileLink", "file", "multihopDir", "brokenLink", "dir", "multihopFile", "loopLink", "nonExistent", NULL };
    const char *expected_file_names[] = { "brokenLink", "file", "fileLink", "loopLink", "multihopFile" };
    const char *expected_dir_headers[] = { "dir", "dirLink", "multihopDir" };
    const char *expected_dir_contents[] = { "dirFile", "symDir", "symlink" };
    get_parsed_arguments_result(9, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n%s\n",
        expected_file_names[0],
        expected_file_names[1],
        expected_file_names[2],
        expected_file_names[3],
        expected_file_names[4],
        expected_dir_headers[0],
        expected_dir_contents[0],
        expected_dir_contents[1],
        expected_dir_contents[2],
        expected_dir_headers[1],
        expected_dir_contents[0],
        expected_dir_contents[1],
        expected_dir_contents[2],
        expected_dir_headers[2],
        expected_dir_contents[0],
        expected_dir_contents[1],
        expected_dir_contents[2]
    ));
    CU_ASSERT(verify_that_the_error_printed_is("ft_ls: cannot access '%s': %s\n", arguments[8], strerror(ENOENT)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_successfully_print_the_contents_of_the_current_directory_recursively(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file", "subdir", "file2", "subdir2", "symlink"),
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_SYMLINK("symlink", "subdir"),
        MOCK_DIR("subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_DIR("subdir2", ".", "..", "sbd_dir2"),
        MOCK_DIR("./subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_DIR("./subdir2", ".", "..", "sbd_dir2"),
        MOCK_FILE("./subdir/sbd_file1"),
        MOCK_DIR("./subdir/sbd_dir1", ".", ".."),
        MOCK_DIR("./subdir2/sbd_dir2", ".", "..", "char", "block"),
        MOCK_CHAR_DEVICE("./subdir2/sbd_dir2/char"),
        MOCK_BLOCK_DEVICE("./subdir2/sbd_dir2/block"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-R", NULL };
    const char *expected_file_names[] = { "file", "file2", "symlink", "sbd_file1", "block", "char" };
    const char *expected_dir_headers[] = { ".", "./subdir", "./subdir/sbd_dir1", "./subdir2", "./subdir2/sbd_dir2" };
    const char *expected_subdirs[] = { "subdir", "subdir2", "sbd_dir1", "sbd_dir2" };
    get_parsed_arguments_result(1, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s:\n"
        "%s\n%s\n%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "\n%s:\n"
        "%s\n"
        "\n%s:\n"
        "%s\n%s\n",
        expected_dir_headers[0],
        expected_file_names[0],
        expected_file_names[1],
        expected_subdirs[0],
        expected_subdirs[1],
        expected_file_names[2],
        expected_dir_headers[1],
        expected_subdirs[2],
        expected_file_names[3],
        expected_dir_headers[2],
        expected_dir_headers[3],
        expected_subdirs[3],
        expected_dir_headers[4],
        expected_file_names[4],
        expected_file_names[5]
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_SUCCESS);
}

static void should_successfully_print_the_contents_of_the_specified_non_directory_even_with_recursive_option(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "file", "-R", NULL };
    get_parsed_arguments_result(2, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n",
        arguments[0]
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_SUCCESS);
}

static void should_successfully_print_the_contents_of_the_specified_directory_recursively(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "..", "file", "subdir", "file2", "subdir2", "symlink"),
        MOCK_FILE("dir/file"),
        MOCK_FILE("dir/file2"),
        MOCK_SYMLINK("dir/symlink", "dir/subdir"),
        MOCK_DIR("dir/subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_DIR("dir/subdir2", ".", "..", "sbd_dir2"),
        MOCK_FILE("dir/subdir/sbd_file1"),
        MOCK_DIR("dir/subdir/sbd_dir1", ".", ".."),
        MOCK_DIR("dir/subdir2/sbd_dir2", ".", "..", "char", "block"),
        MOCK_CHAR_DEVICE("dir/subdir2/sbd_dir2/char"),
        MOCK_BLOCK_DEVICE("dir/subdir2/sbd_dir2/block"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-R", "dir", NULL };
    const char *expected_file_names[] = { "file", "file2", "symlink", "sbd_file1", "block", "char" };
    const char *expected_dir_headers[] = { "dir", "dir/subdir", "dir/subdir/sbd_dir1", "dir/subdir2", "dir/subdir2/sbd_dir2" };
    const char *expected_subdirs[] = { "subdir", "subdir2", "sbd_dir1", "sbd_dir2" };
    get_parsed_arguments_result(2, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s:\n"
        "%s\n%s\n%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "\n%s:\n"
        "%s\n"
        "\n%s:\n"
        "%s\n%s\n",
        expected_dir_headers[0],
        expected_file_names[0],
        expected_file_names[1],
        expected_subdirs[0],
        expected_subdirs[1],
        expected_file_names[2],
        expected_dir_headers[1],
        expected_subdirs[2],
        expected_file_names[3],
        expected_dir_headers[2],
        expected_dir_headers[3],
        expected_subdirs[3],
        expected_dir_headers[4],
        expected_file_names[4],
        expected_file_names[5]
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_SUCCESS);
}

static void should_successfully_print_the_contents_of_the_specified_operands_recursively(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("nonDir"),
        MOCK_FILE("nonDir2"),
        MOCK_DIR("dir", ".", "..", "file", "subdir", "file2", "subdir2", "symlink"),
        MOCK_FILE("dir/file"),
        MOCK_FILE("dir/file2"),
        MOCK_SYMLINK("dir/symlink", "dir/subdir"),
        MOCK_DIR("dir/subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_DIR("dir/subdir2", ".", "..", "sbd_dir2"),
        MOCK_FILE("dir/subdir/sbd_file1"),
        MOCK_DIR("dir/subdir/sbd_dir1", ".", ".."),
        MOCK_DIR("dir/subdir2/sbd_dir2", ".", "..", "char", "block"),
        MOCK_CHAR_DEVICE("dir/subdir2/sbd_dir2/char"),
        MOCK_BLOCK_DEVICE("dir/subdir2/sbd_dir2/block"),
        MOCK_DIR("dir2/", ".", "..", "file", "subdirDir2"),
        MOCK_FILE("dir2/file"),
        MOCK_DIR("dir2/subdirDir2", ".", "..", "file2"),
        MOCK_FILE("dir2/subdirDir2/file2"),
        MOCK_SYMLINK("symlink", "dir2/"),
        MOCK_FILE("symlink/file"),
        MOCK_DIR("symlink/subdirDir2", ".", "..", "file2"),
        MOCK_FILE("symlink/subdirDir2/file2"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "nonDir", "dir2/", "nonDir2", "-R", "dir", "symlink", NULL };
    const char *expected_file_names[] = { "file", "file2", "symlink", "sbd_file1", "block", "char" };
    const char *expected_dir_headers[] = { "dir", "dir/subdir", "dir/subdir/sbd_dir1", "dir/subdir2", "dir/subdir2/sbd_dir2", "dir2/", "dir2/subdirDir2" };
    const char *expected_symlink_headers[] = { "symlink", "symlink/subdirDir2" };
    const char *expected_subdirs[] = { "subdir", "subdir2", "sbd_dir1", "sbd_dir2", "subdirDir2" };
    const char *expected_symlink_subdirs[] = { "subdirDir2" };
    get_parsed_arguments_result(6, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "\n%s:\n"
        "%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n",
        arguments[0],
        arguments[2],
        expected_dir_headers[0],
        expected_file_names[0],
        expected_file_names[1],
        expected_subdirs[0],
        expected_subdirs[1],
        expected_file_names[2],
        expected_dir_headers[1],
        expected_subdirs[2],
        expected_file_names[3],
        expected_dir_headers[2],
        expected_dir_headers[3],
        expected_subdirs[3],
        expected_dir_headers[4],
        expected_file_names[4],
        expected_file_names[5],
        expected_dir_headers[5],
        expected_file_names[0],
        expected_subdirs[4],
        expected_dir_headers[6],
        expected_file_names[1],
        expected_symlink_headers[0],
        expected_file_names[0],
        expected_symlink_subdirs[0],
        expected_symlink_headers[1],
        expected_file_names[1]
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_SUCCESS);
}

static void should_render_the_default_directory_in_long_format(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file", "large", "link"),
        MOCK_FILE_WITH_STATS("file", MOCK_BLOCKS(2), MOCK_HARDLINK_COUNT(3), MOCK_AUTHOR("alice"), MOCK_GROUP("staff"), MOCK_SIZE(42), MOCK_MODIFICATION_TIME(2, 10, 2000, 12, 30, 0)),
        MOCK_FILE_WITH_STATS("large", MOCK_BLOCKS(7), MOCK_HARDLINK_COUNT(12), MOCK_AUTHOR("charlie"), MOCK_GROUP("developers"), MOCK_SIZE(12345), MOCK_MODIFICATION_TIME(12, 11, 2001, 8, 5, 0)),
        MOCK_SYMLINK_WITH_STATS("link", "file", MOCK_BLOCKS(1), MOCK_HARDLINK_COUNT(1), MOCK_AUTHOR("bob"), MOCK_GROUP("wheel"), MOCK_SIZE(4), MOCK_MODIFICATION_TIME(1, 1, 2002, 9, 15, 0)),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-l", NULL };
    get_parsed_arguments_result(1, arguments);
    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "total 5\n"
        "-rw-r--r--  3 alice   staff         42 Oct  2  2000 file\n"
        "-rw-r--r-- 12 charlie developers 12345 Nov 12  2001 large\n"
        "lrwxrwxrwx  1 bob     wheel          4 Jan  1  2002 link -> file\n"));
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_render_a_non_directory_operand_in_long_format(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_WITH_STATS("file", MOCK_BLOCKS(2), MOCK_HARDLINK_COUNT(3), MOCK_AUTHOR("alice"), MOCK_GROUP("staff"), MOCK_SIZE(42), MOCK_MODIFICATION_TIME(2, 10, 2000, 12, 30, 0)),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-l", "file", NULL };
    get_parsed_arguments_result(2, arguments);
    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "-rw-r--r-- 3 alice staff 42 Oct  2  2000 file\n"
    ));
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_render_a_symlink_to_a_directory_operand_as_a_link_in_long_format(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_SYMLINK_WITH_STATS("link", "dir",
            MOCK_BLOCKS(1),
            MOCK_HARDLINK_COUNT(4),
            MOCK_AUTHOR("alice"),
            MOCK_GROUP("staff"),
            MOCK_SIZE(3),
            MOCK_MODIFICATION_TIME(2, 10, 2000, 12, 30, 0)
        ),
        MOCK_DIR("dir", ".", "..", "inside"),
        MOCK_FILE("dir/inside"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-l", "link", NULL };
    get_parsed_arguments_result(2, arguments);
    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "lrwxrwxrwx 4 alice staff 3 Oct  2  2000 link -> dir\n"));
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_render_a_directory_operand_in_long_format(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "..", "file", "large", "link"),
        MOCK_FILE_WITH_STATS("dir/file", MOCK_BLOCKS(2), MOCK_HARDLINK_COUNT(3), MOCK_AUTHOR("alice"), MOCK_GROUP("staff"), MOCK_SIZE(42), MOCK_MODIFICATION_TIME(2, 10, 2000, 12, 30, 0)),
        MOCK_FILE_WITH_STATS("dir/large", MOCK_BLOCKS(7), MOCK_HARDLINK_COUNT(12), MOCK_AUTHOR("charlie"), MOCK_GROUP("developers"), MOCK_SIZE(12345), MOCK_MODIFICATION_TIME(12, 11, 2001, 8, 5, 0)),
        MOCK_SYMLINK_WITH_STATS("dir/link", "file", MOCK_BLOCKS(1), MOCK_HARDLINK_COUNT(1), MOCK_AUTHOR("bob"), MOCK_GROUP("wheel"), MOCK_SIZE(4), MOCK_MODIFICATION_TIME(1, 1, 2002, 9, 15, 0)),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-l", "dir", NULL };
    get_parsed_arguments_result(2, arguments);
    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "total 5\n"
        "-rw-r--r--  3 alice   staff         42 Oct  2  2000 file\n"
        "-rw-r--r-- 12 charlie developers 12345 Nov 12  2001 large\n"
        "lrwxrwxrwx  1 bob     wheel          4 Jan  1  2002 link -> file\n"));
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_render_mixed_operands_in_long_format(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_WITH_STATS("file", MOCK_BLOCKS(2), MOCK_HARDLINK_COUNT(3), MOCK_AUTHOR("alice"), MOCK_GROUP("staff"), MOCK_SIZE(42), MOCK_MODIFICATION_TIME(2, 10, 2000, 12, 30, 0)),
        MOCK_FILE_WITH_STATS("file2", MOCK_BLOCKS(7), MOCK_HARDLINK_COUNT(12), MOCK_AUTHOR("charlie"), MOCK_GROUP("developers"), MOCK_SIZE(12345), MOCK_MODIFICATION_TIME(12, 11, 2001, 8, 5, 0)),
        MOCK_DIR_WITH_STATS("dir", ((const char *[]){ "inside", "link", NULL }), MOCK_BLOCKS(1)),
        MOCK_FILE_WITH_STATS("dir/inside", MOCK_BLOCKS(3), MOCK_HARDLINK_COUNT(5), MOCK_AUTHOR("bob"), MOCK_GROUP("wheel"), MOCK_SIZE(999), MOCK_MODIFICATION_TIME(1, 1, 2002, 9, 15, 0)),
        MOCK_SYMLINK_WITH_STATS("dir/link", "inside", MOCK_BLOCKS(1), MOCK_HARDLINK_COUNT(1), MOCK_AUTHOR("alice"), MOCK_GROUP("staff"), MOCK_SIZE(4), MOCK_MODIFICATION_TIME(2, 10, 2000, 12, 30, 0)),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-l", "file", "file2", "dir", NULL };
    get_parsed_arguments_result(4, arguments);
    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "-rw-r--r--  3 alice   staff         42 Oct  2  2000 file\n"
        "-rw-r--r-- 12 charlie developers 12345 Nov 12  2001 file2\n"
        "\n"
        "dir:\n"
        "total 2\n"
        "-rw-r--r-- 5 bob   wheel 999 Jan  1  2002 inside\n"
        "lrwxrwxrwx 1 alice staff   4 Oct  2  2000 link -> inside\n"));
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_render_file_stats_for_non_directory_and_directory_entries_in_long_format(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_WITH_STATS("file",
            MOCK_BLOCKS(2),
            MOCK_HARDLINK_COUNT(3),
            MOCK_AUTHOR("alice"),
            MOCK_GROUP("staff"),
            MOCK_SIZE(42),
            MOCK_MODIFICATION_TIME(2, 10, 2000, 12, 30, 0)
        ),
        MOCK_FILE_WITH_STATS("file2",
            MOCK_BLOCKS(3),
            MOCK_HARDLINK_COUNT(12),
            MOCK_AUTHOR("charlie"),
            MOCK_GROUP("developers"),
            MOCK_SIZE(12345),
            MOCK_MODIFICATION_TIME(2, 10, 2000, 12, 30, 0)
        ),
        MOCK_DIR_WITH_STATS("dir", ((const char *[]){ "device", "link", NULL }),
            MOCK_BLOCKS(1)
        ),
        MOCK_CHAR_DEVICE_WITH_STATS("dir/device",
            MOCK_BLOCKS(4),
            MOCK_HARDLINK_COUNT(5),
            MOCK_AUTHOR("bob"),
            MOCK_GROUP("wheel"),
            MOCK_DEVICE(12, 34),
            MOCK_MODIFICATION_TIME(2, 10, 2000, 12, 30, 0)
        ),
        MOCK_SYMLINK_WITH_STATS("dir/link", "file",
            MOCK_BLOCKS(2),
            MOCK_HARDLINK_COUNT(1),
            MOCK_AUTHOR("alice"),
            MOCK_GROUP("staff"),
            MOCK_SIZE(4),
            MOCK_MODIFICATION_TIME(2, 10, 2000, 12, 30, 0)
        ),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-l", "file", "file2", "dir", NULL };
    get_parsed_arguments_result(4, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n"
        "%s\n"
        "\n"
        "%s:\n"
        "%s\n"
        "%s\n"
        "%s\n",
        "-rw-r--r--  3 alice   staff         42 Oct  2  2000 file",
        "-rw-r--r-- 12 charlie developers 12345 Oct  2  2000 file2",
        "dir",
        "total 3",
        "crw-rw---- 5 bob   wheel 12, 34 Oct  2  2000 device",
        "lrwxrwxrwx 1 alice staff      4 Oct  2  2000 link -> file"));
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_fall_back_to_numeric_author_and_group_in_long_format(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_WITH_STATS("file", MOCK_UID(12345), MOCK_GID(23456), MOCK_SIZE(42), MOCK_HARDLINK_COUNT(2), MOCK_MODIFICATION_TIME(2, 10, 2000, 12, 30, 0)),
        MOCK_FILE_WITH_STATS("named", MOCK_AUTHOR("alice"), MOCK_GROUP("developers"), MOCK_SIZE(12345), MOCK_HARDLINK_COUNT(12), MOCK_MODIFICATION_TIME(12, 11, 2001, 8, 5, 0)),
        MOCK_CHAR_DEVICE_WITH_STATS("device", MOCK_UID(12345), MOCK_GID(23456), MOCK_DEVICE(12, 34), MOCK_HARDLINK_COUNT(5), MOCK_MODIFICATION_TIME(1, 1, 2002, 9, 15, 0)),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-l", "file", "named", "device", NULL };
    get_parsed_arguments_result(4, arguments);
    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "crw-rw----  5 12345 23456      12, 34 Jan  1  2002 device\n"
        "-rw-r--r--  2 12345 23456          42 Oct  2  2000 file\n"
        "-rw-r--r-- 12 alice developers  12345 Nov 12  2001 named\n"));
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_report_a_minor_error_when_lstat_fails_for_directory_content(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "..", "failed", "good", "link"),
        {
            .path = "dir/failed",
            .stats = { .st_mode = DEFAULT_FILE_MODE },
            .errors = { .lstat_errno = EACCES }
        },
        MOCK_FILE_WITH_STATS("dir/good", MOCK_BLOCKS(5), MOCK_HARDLINK_COUNT(12), MOCK_AUTHOR("charlie"), MOCK_GROUP("developers"), MOCK_SIZE(12345), MOCK_MODIFICATION_TIME(12, 11, 2001, 8, 5, 0)),
        MOCK_SYMLINK_WITH_STATS("dir/link", "good", MOCK_BLOCKS(2), MOCK_HARDLINK_COUNT(3), MOCK_AUTHOR("alice"), MOCK_GROUP("staff"), MOCK_SIZE(4), MOCK_MODIFICATION_TIME(2, 10, 2000, 12, 30, 0)),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-l", "dir", NULL };
    get_parsed_arguments_result(2, arguments);
    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "total 4\n"
        "?---------  ? ?       ?              ? ?            failed\n"
        "-rw-r--r-- 12 charlie developers 12345 Nov 12  2001 good\n"
        "lrwxrwxrwx  3 alice   staff          4 Oct  2  2000 link -> good\n"));
    CU_ASSERT(verify_that_the_error_printed_is("ft_ls: cannot access '%s': %s\n", "dir/failed", strerror(EACCES)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MINOR_ERROR);
}

static void should_report_a_minor_error_when_readlink_fails_for_directory_content(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "..", "link", "good"),
        MOCK_READ_SYM_LINK_ERROR(EACCES, "dir/link", "target"),
        MOCK_FILE_WITH_STATS("dir/good", MOCK_BLOCKS(5), MOCK_HARDLINK_COUNT(12), MOCK_AUTHOR("charlie"), MOCK_GROUP("developers"), MOCK_SIZE(12345), MOCK_MODIFICATION_TIME(12, 11, 2001, 8, 5, 0)),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-l", "dir", NULL };
    get_parsed_arguments_result(2, arguments);
    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "total 3\n"
        "-rw-r--r-- 12 charlie developers 12345 Nov 12  2001 good\n"
        "lrwxrwxrwx  1 root    root        1024 Jan  1  1970 link\n"));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot read symbolic link '%s': %s\n",
        "dir/link", strerror(EACCES)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MINOR_ERROR);
}

static void should_report_a_major_error_when_stat_fails_for_an_operand_in_long_format(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_ACCESS_ERROR(EACCES, "failed"),
        MOCK_FILE_WITH_STATS("good", MOCK_BLOCKS(5), MOCK_HARDLINK_COUNT(12), MOCK_AUTHOR("charlie"), MOCK_GROUP("developers"), MOCK_SIZE(12345), MOCK_MODIFICATION_TIME(12, 11, 2001, 8, 5, 0)),
        MOCK_CHAR_DEVICE_WITH_STATS("device", MOCK_BLOCKS(2), MOCK_HARDLINK_COUNT(3), MOCK_AUTHOR("alice"), MOCK_GROUP("staff"), MOCK_DEVICE(12, 34), MOCK_MODIFICATION_TIME(2, 10, 2000, 12, 30, 0)),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-l", "failed", "good", "device", NULL };
    get_parsed_arguments_result(4, arguments);
    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "crw-rw----  3 alice   staff      12, 34 Oct  2  2000 device\n"
        "-rw-r--r-- 12 charlie developers  12345 Nov 12  2001 good\n"));
    CU_ASSERT(verify_that_the_error_printed_is("ft_ls: cannot access '%s': %s\n", "failed", strerror(EACCES)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_report_a_major_error_when_readlink_fails_for_an_operand_in_long_format(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_READ_SYM_LINK_ERROR(EACCES, "link", "target"),
        MOCK_FILE_WITH_STATS("good", MOCK_BLOCKS(5), MOCK_HARDLINK_COUNT(12), MOCK_AUTHOR("charlie"), MOCK_GROUP("developers"), MOCK_SIZE(12345), MOCK_MODIFICATION_TIME(12, 11, 2001, 8, 5, 0)),
        MOCK_SYMLINK_WITH_STATS("link2", "good", MOCK_BLOCKS(2), MOCK_HARDLINK_COUNT(3), MOCK_AUTHOR("alice"), MOCK_GROUP("staff"), MOCK_SIZE(4), MOCK_MODIFICATION_TIME(2, 10, 2000, 12, 30, 0)),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-l", "link", "good", "link2", NULL };
    get_parsed_arguments_result(4, arguments);
    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "-rw-r--r-- 12 charlie developers 12345 Nov 12  2001 good\n"
        "lrwxrwxrwx  1 root    root        1024 Jan  1  1970 link\n"
        "lrwxrwxrwx  3 alice   staff          4 Oct  2  2000 link2 -> good\n"));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot read symbolic link '%s': %s\n",
        "link", strerror(EACCES)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_report_multiple_minor_errors_for_lstat_failures_in_one_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "..", "failed1", "failed2", "good"),
        {
            .path = "dir/failed1",
            .stats = { .st_mode = DEFAULT_FILE_MODE },
            .errors = { .lstat_errno = EACCES }
        },
        {
            .path = "dir/failed2",
            .stats = { .st_mode = DEFAULT_FILE_MODE },
            .errors = { .lstat_errno = ENOENT }
        },
        MOCK_FILE_WITH_STATS("dir/good", MOCK_BLOCKS(5), MOCK_HARDLINK_COUNT(12), MOCK_AUTHOR("charlie"), MOCK_GROUP("developers"), MOCK_SIZE(12345), MOCK_MODIFICATION_TIME(12, 11, 2001, 8, 5, 0)),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-l", "dir", NULL };
    get_parsed_arguments_result(2, arguments);
    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "total 3\n"
        "?---------  ? ?       ?              ? ?            failed1\n"
        "?---------  ? ?       ?              ? ?            failed2\n"
        "-rw-r--r-- 12 charlie developers 12345 Nov 12  2001 good\n"));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot access '%s': %s\n",
        "dir/failed1", strerror(EACCES),
        "dir/failed2", strerror(ENOENT)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MINOR_ERROR);
}

static void should_report_multiple_minor_errors_for_lstat_and_readlink_failures(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "..", "failed", "link", "good"),
        {
            .path = "dir/failed",
            .stats = { .st_mode = DEFAULT_FILE_MODE },
            .errors = { .lstat_errno = EACCES }
        },
        MOCK_READ_SYM_LINK_ERROR(EINVAL, "dir/link", "target"),
        MOCK_FILE_WITH_STATS("dir/good", MOCK_BLOCKS(5), MOCK_HARDLINK_COUNT(12), MOCK_AUTHOR("charlie"), MOCK_GROUP("developers"), MOCK_SIZE(12345), MOCK_MODIFICATION_TIME(12, 11, 2001, 8, 5, 0)),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-l", "dir", NULL };
    get_parsed_arguments_result(2, arguments);
    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "total 3\n"
        "?---------  ? ?       ?              ? ?            failed\n"
        "-rw-r--r-- 12 charlie developers 12345 Nov 12  2001 good\n"
        "lrwxrwxrwx  1 root    root        1024 Jan  1  1970 link\n"));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot read symbolic link '%s': %s\n",
        "dir/failed", strerror(EACCES),
        "dir/link", strerror(EINVAL)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MINOR_ERROR);
}

static void should_report_multiple_major_errors_for_stat_failures_on_operands(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_ACCESS_ERROR(EACCES, "failed1"),
        MOCK_FILE_ACCESS_ERROR(ENOENT, "failed2"),
        MOCK_FILE_WITH_STATS("good", MOCK_BLOCKS(5), MOCK_HARDLINK_COUNT(12), MOCK_AUTHOR("charlie"), MOCK_GROUP("developers"), MOCK_SIZE(12345), MOCK_MODIFICATION_TIME(12, 11, 2001, 8, 5, 0)),
        MOCK_CHAR_DEVICE_WITH_STATS("device", MOCK_BLOCKS(2), MOCK_HARDLINK_COUNT(3), MOCK_AUTHOR("alice"), MOCK_GROUP("staff"), MOCK_DEVICE(12, 34), MOCK_MODIFICATION_TIME(2, 10, 2000, 12, 30, 0)),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-l", "failed1", "failed2", "good", "device", NULL };
    get_parsed_arguments_result(5, arguments);
    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "crw-rw----  3 alice   staff      12, 34 Oct  2  2000 device\n"
        "-rw-r--r-- 12 charlie developers  12345 Nov 12  2001 good\n"));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot access '%s': %s\n",
        "failed1", strerror(EACCES),
        "failed2", strerror(ENOENT)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_report_multiple_major_errors_for_stat_and_readlink_failures_on_operands(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_ACCESS_ERROR(EACCES, "failed"),
        MOCK_READ_SYM_LINK_ERROR(EINVAL, "link", "target"),
        MOCK_FILE_WITH_STATS("good", MOCK_BLOCKS(5), MOCK_HARDLINK_COUNT(12), MOCK_AUTHOR("charlie"), MOCK_GROUP("developers"), MOCK_SIZE(12345), MOCK_MODIFICATION_TIME(12, 11, 2001, 8, 5, 0)),
        MOCK_SYMLINK_WITH_STATS("link2", "good", MOCK_BLOCKS(2), MOCK_HARDLINK_COUNT(3), MOCK_AUTHOR("alice"), MOCK_GROUP("staff"), MOCK_SIZE(4), MOCK_MODIFICATION_TIME(2, 10, 2000, 12, 30, 0)),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-l", "failed", "link", "good", "link2", NULL };
    get_parsed_arguments_result(5, arguments);
    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "-rw-r--r-- 12 charlie developers 12345 Nov 12  2001 good\n"
        "lrwxrwxrwx  1 root    root        1024 Jan  1  1970 link\n"
        "lrwxrwxrwx  3 alice   staff          4 Oct  2  2000 link2 -> good\n"));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot read symbolic link '%s': %s\n",
        "failed", strerror(EACCES),
        "link", strerror(EINVAL)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_report_major_and_minor_errors_for_operands_and_directory_content(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_ACCESS_ERROR(EACCES, "failed"),
        MOCK_FILE_WITH_STATS("good", MOCK_BLOCKS(5), MOCK_HARDLINK_COUNT(12), MOCK_AUTHOR("charlie"), MOCK_GROUP("developers"), MOCK_SIZE(12345), MOCK_MODIFICATION_TIME(12, 11, 2001, 8, 5, 0)),
        MOCK_DIR("dir", ".", "..", "failed", "inside"),
        {
            .path = "dir/failed",
            .stats = { .st_mode = DEFAULT_FILE_MODE },
            .errors = { .lstat_errno = ENOENT }
        },
        MOCK_SYMLINK_WITH_STATS("dir/inside", "good", MOCK_BLOCKS(2), MOCK_HARDLINK_COUNT(3), MOCK_AUTHOR("alice"), MOCK_GROUP("staff"), MOCK_SIZE(4), MOCK_MODIFICATION_TIME(2, 10, 2000, 12, 30, 0)),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-l", "failed", "good", "dir", NULL };
    get_parsed_arguments_result(4, arguments);
    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "-rw-r--r-- 12 charlie developers 12345 Nov 12  2001 good\n"
        "\n"
        "dir:\n"
        "total 1\n"
        "?--------- ? ?     ?     ? ?            failed\n"
        "lrwxrwxrwx 3 alice staff 4 Oct  2  2000 inside -> good\n"));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot access '%s': %s\n",
        "failed", strerror(EACCES),
        "dir/failed", strerror(ENOENT)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_render_broken_looped_and_multihop_links_in_long_format(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("target"),
        MOCK_ENTRY("broken", DEFAULT_SYMLINK_MODE, NULL, "missing", ((t_vfs_mock_errors){ .stat_errno = ENOENT })),
        MOCK_ENTRY("looped", DEFAULT_SYMLINK_MODE, NULL, "looped", ((t_vfs_mock_errors){ .stat_errno = ELOOP })),
        MOCK_SYMLINK("multihop", "second-hop"),
        MOCK_SYMLINK("second-hop", "target"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-l", "broken", "looped", "multihop", NULL };
    get_parsed_arguments_result(4, arguments);
    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "lrwxrwxrwx 1 root root 1024 Jan  1  1970 broken -> missing\n"
        "lrwxrwxrwx 1 root root 1024 Jan  1  1970 looped -> looped\n"
        "lrwxrwxrwx 1 root root 1024 Jan  1  1970 multihop -> second-hop\n"));
    CU_ASSERT(verify_that_no_error_was_printed());
    assert_application_execution_succeed(result);
}

static void should_print_visible_and_hidden_entries_in_the_current_directory_with_read_all(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "visible", ".env"),
        MOCK_DIR("..", ".", ".."),
        MOCK_FILE("visible"),
        MOCK_FILE(".env"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-a", NULL };
    get_parsed_arguments_result(1, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(result, ".\n..\n.env\nvisible\n");
}

static void should_print_visible_entries_in_the_current_directory_without_hidden_entries_with_read_all(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "visible"),
        MOCK_DIR("..", ".", ".."),
        MOCK_FILE("visible"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-a", NULL };
    get_parsed_arguments_result(1, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(result, ".\n..\nvisible\n");
}

static void should_print_only_dot_entries_for_an_empty_current_directory_with_read_all(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", ".."),
        MOCK_DIR("..", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-a", NULL };
    get_parsed_arguments_result(1, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(result, ".\n..\n");
}

static void should_print_hidden_entries_in_a_current_directory_with_only_hidden_entries_with_read_all(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", ".env", ".config"),
        MOCK_DIR("..", ".", ".."),
        MOCK_FILE(".env"),
        MOCK_FILE(".config"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-a", NULL };
    get_parsed_arguments_result(1, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(result, ".\n..\n.config\n.env\n");
}

static void should_print_visible_and_hidden_entries_in_a_directory_operand_with_read_all(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "..", "visible", ".env"),
        MOCK_DIR("dir/.", ".", ".."),
        MOCK_DIR("dir/..", ".", ".."),
        MOCK_FILE("dir/visible"),
        MOCK_FILE("dir/.env"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-a", "dir", NULL };
    get_parsed_arguments_result(2, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(result, ".\n..\n.env\nvisible\n");
}

static void should_print_visible_entries_in_a_directory_operand_without_hidden_entries_with_read_all(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "..", "visible"),
        MOCK_DIR("dir/.", ".", ".."),
        MOCK_DIR("dir/..", ".", ".."),
        MOCK_FILE("dir/visible"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-a", "dir", NULL };
    get_parsed_arguments_result(2, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(result, ".\n..\nvisible\n");
}

static void should_print_only_dot_entries_for_an_empty_directory_operand_with_read_all(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("empty", ".", ".."),
        MOCK_DIR("empty/.", ".", ".."),
        MOCK_DIR("empty/..", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-a", "empty", NULL };
    get_parsed_arguments_result(2, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(result, ".\n..\n");
}

static void should_print_hidden_entries_in_a_directory_operand_with_only_hidden_entries_with_read_all(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "..", ".env", ".config"),
        MOCK_DIR("dir/.", ".", ".."),
        MOCK_DIR("dir/..", ".", ".."),
        MOCK_FILE("dir/.env"),
        MOCK_FILE("dir/.config"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-a", "dir", NULL };
    get_parsed_arguments_result(2, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(result, ".\n..\n.config\n.env\n");
}

static void should_print_hidden_and_visible_entries_from_multiple_directory_operands_with_read_all(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir1", ".", "..", ".env", "visible"),
        MOCK_DIR("dir1/.", ".", ".."),
        MOCK_DIR("dir1/..", ".", ".."),
        MOCK_DIR("dir2", ".", "..", "normal"),
        MOCK_DIR("dir2/.", ".", ".."),
        MOCK_DIR("dir2/..", ".", ".."),
        MOCK_FILE("dir1/.env"),
        MOCK_FILE("dir1/visible"),
        MOCK_FILE("dir2/normal"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-a", "dir1", "dir2", NULL };
    get_parsed_arguments_result(3, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(
        result,
        "dir1:\n"
        ".\n"
        "..\n"
        ".env\n"
        "visible\n"
        "\n"
        "dir2:\n"
        ".\n"
        "..\n"
        "normal\n"
    );
}

static void should_print_headers_for_empty_and_hidden_only_directory_operands_with_read_all(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("empty", ".", ".."),
        MOCK_DIR("empty/.", ".", ".."),
        MOCK_DIR("empty/..", ".", ".."),
        MOCK_DIR("hidden", ".", "..", ".env"),
        MOCK_DIR("hidden/.", ".", ".."),
        MOCK_DIR("hidden/..", ".", ".."),
        MOCK_FILE("hidden/.env"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-a", "empty", "hidden", NULL };
    get_parsed_arguments_result(3, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(
        result,
        "empty:\n"
        ".\n"
        "..\n"
        "\n"
        "hidden:\n"
        ".\n"
        "..\n"
        ".env\n"
    );
}

static void should_print_hidden_and_visible_entries_from_mixed_operands_with_read_all(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE(".env"),
        MOCK_FILE("file"),
        MOCK_DIR("dir", ".", "..", ".hidden", "visible"),
        MOCK_DIR("dir/.", ".", ".."),
        MOCK_DIR("dir/..", ".", ".."),
        MOCK_FILE("dir/.hidden"),
        MOCK_FILE("dir/visible"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-a", ".env", "dir", "file", NULL };
    get_parsed_arguments_result(4, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(
        result,
        ".env\n"
        "file\n"
        "\n"
        "dir:\n"
        ".\n"
        "..\n"
        ".hidden\n"
        "visible\n"
    );
}

static void should_print_mixed_operands_with_empty_and_hidden_only_directories_with_read_all(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file"),
        MOCK_DIR("empty", ".", ".."),
        MOCK_DIR("empty/.", ".", ".."),
        MOCK_DIR("empty/..", ".", ".."),
        MOCK_DIR("hidden", ".", "..", ".env"),
        MOCK_DIR("hidden/.", ".", ".."),
        MOCK_DIR("hidden/..", ".", ".."),
        MOCK_FILE("hidden/.env"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-a", "file", "empty", "hidden", NULL };
    get_parsed_arguments_result(4, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(
        result,
        "file\n"
        "\n"
        "empty:\n"
        ".\n"
        "..\n"
        "\n"
        "hidden:\n"
        ".\n"
        "..\n"
        ".env\n"
    );
}

static void should_print_headers_for_all_mixed_operand_directories_with_read_all(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE(".env"),
        MOCK_FILE("file"),
        MOCK_DIR("dir", ".", "..", ".hidden", "visible"),
        MOCK_DIR("dir/.", ".", ".."),
        MOCK_DIR("dir/..", ".", ".."),
        MOCK_FILE("dir/.hidden"),
        MOCK_FILE("dir/visible"),
        MOCK_DIR("empty", ".", ".."),
        MOCK_DIR("empty/.", ".", ".."),
        MOCK_DIR("empty/..", ".", ".."),
        MOCK_DIR("only_hidden", ".", "..", ".secret"),
        MOCK_DIR("only_hidden/.", ".", ".."),
        MOCK_DIR("only_hidden/..", ".", ".."),
        MOCK_FILE("only_hidden/.secret"),
        MOCK_DIR("plain_dir", ".", "..", "normal"),
        MOCK_DIR("plain_dir/.", ".", ".."),
        MOCK_DIR("plain_dir/..", ".", ".."),
        MOCK_FILE("plain_dir/normal"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-a", ".env", "dir", "empty", "file", "only_hidden", "plain_dir", NULL };
    get_parsed_arguments_result(7, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(
        result,
        ".env\n"
        "file\n"
        "\n"
        "dir:\n"
        ".\n"
        "..\n"
        ".hidden\n"
        "visible\n"
        "\n"
        "empty:\n"
        ".\n"
        "..\n"
        "\n"
        "only_hidden:\n"
        ".\n"
        "..\n"
        ".secret\n"
        "\n"
        "plain_dir:\n"
        ".\n"
        "..\n"
        "normal\n"
    );
}

static void should_print_explicitly_specified_hidden_file_and_directory_operands_with_read_all(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE(".env"),
        MOCK_DIR(".idea", ".", "..", ".settings", "visible"),
        MOCK_DIR(".idea/.", ".", ".."),
        MOCK_DIR(".idea/..", ".", ".."),
        MOCK_FILE(".idea/.settings"),
        MOCK_FILE(".idea/visible"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-a", ".env", ".idea", NULL };
    get_parsed_arguments_result(3, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(
        result,
        ".env\n"
        "\n"
        ".idea:\n"
        ".\n"
        "..\n"
        ".settings\n"
        "visible\n"
    );
}

static void should_reverse_sort_the_current_directory_when_reverse_option_is_active(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "alpha", "middle", "zeta"),
        MOCK_FILE("alpha"),
        MOCK_FILE("middle"),
        MOCK_FILE("zeta"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-r", NULL };
    get_parsed_arguments_result(1, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(result, "zeta\nmiddle\nalpha\n");
}

static void should_reverse_sort_a_directory_operand_contents_when_reverse_option_is_active(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "..", "alpha", "middle", "zeta"),
        MOCK_FILE("dir/alpha"),
        MOCK_FILE("dir/middle"),
        MOCK_FILE("dir/zeta"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-r", "dir", NULL };
    get_parsed_arguments_result(2, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(result, "zeta\nmiddle\nalpha\n");
}

static void should_reverse_sort_multiple_file_operands_when_reverse_option_is_active(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("alpha"),
        MOCK_FILE("middle"),
        MOCK_FILE("zeta"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-r", "alpha", "zeta", "middle", NULL };
    get_parsed_arguments_result(4, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(result, "zeta\nmiddle\nalpha\n");
}

static void should_reverse_sort_multiple_directory_operands_and_their_contents_when_reverse_option_is_active(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dirA", ".", "..", "alpha", "zeta"),
        MOCK_DIR("dirZ", ".", "..", "middle", "zeta"),
        MOCK_FILE("dirA/alpha"),
        MOCK_FILE("dirA/zeta"),
        MOCK_FILE("dirZ/middle"),
        MOCK_FILE("dirZ/zeta"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-r", "dirA", "dirZ", NULL };
    get_parsed_arguments_result(3, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(
        result,
        "dirZ:\n"
        "zeta\n"
        "middle\n"
        "\n"
        "dirA:\n"
        "zeta\n"
        "alpha\n"
    );
}

static void should_reverse_sort_file_and_directory_operands_separately_and_directory_contents_when_reverse_option_is_active(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("alpha"),
        MOCK_FILE("middle"),
        MOCK_FILE("zeta"),
        MOCK_DIR("dirA", ".", "..", "alpha", "zeta"),
        MOCK_DIR("dirZ", ".", "..", "middle", "zeta"),
        MOCK_FILE("dirA/alpha"),
        MOCK_FILE("dirA/zeta"),
        MOCK_FILE("dirZ/middle"),
        MOCK_FILE("dirZ/zeta"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-r", "dirA", "alpha", "dirZ", "zeta", "middle", NULL };
    get_parsed_arguments_result(6, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(
        result,
        "zeta\n"
        "middle\n"
        "alpha\n"
        "\n"
        "dirZ:\n"
        "zeta\n"
        "middle\n"
        "\n"
        "dirA:\n"
        "zeta\n"
        "alpha\n"
    );
}

static void should_sort_the_current_directory_by_modification_time_when_no_file_operands_are_specified(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "old", "same_z", "same_a", "new"),
        MOCK_FILE_WITH_STATS("old", MOCK_MODIFICATION_TIME(1, 1, 2020, 0, 0, 0)),
        MOCK_FILE_WITH_STATS("same_z", MOCK_MODIFICATION_TIME(1, 1, 2024, 0, 0, 0)),
        MOCK_FILE_WITH_STATS("same_a", MOCK_MODIFICATION_TIME(1, 1, 2024, 0, 0, 0)),
        MOCK_FILE_WITH_STATS("new", MOCK_MODIFICATION_TIME(1, 1, 2025, 0, 0, 0)),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-t", NULL };
    get_parsed_arguments_result(1, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(result, "new\nsame_a\nsame_z\nold\n");
}

static void should_sort_a_directory_operand_contents_by_modification_time(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR_WITH_STATS("dir", ((const char *[]){ ".", "..", "old", "same_z", "same_a", "new", NULL }), MOCK_MODIFICATION_TIME(1, 1, 2025, 0, 0, 0)),
        MOCK_FILE_WITH_STATS("dir/old", MOCK_MODIFICATION_TIME(1, 1, 2020, 0, 0, 0)),
        MOCK_FILE_WITH_STATS("dir/same_z", MOCK_MODIFICATION_TIME(1, 1, 2024, 0, 0, 0)),
        MOCK_FILE_WITH_STATS("dir/same_a", MOCK_MODIFICATION_TIME(1, 1, 2024, 0, 0, 0)),
        MOCK_FILE_WITH_STATS("dir/new", MOCK_MODIFICATION_TIME(1, 1, 2025, 0, 0, 0)),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-t", "dir", NULL };
    get_parsed_arguments_result(2, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(result, "new\nsame_a\nsame_z\nold\n");
}

static void should_sort_multiple_file_operands_by_modification_time(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_WITH_STATS("old", MOCK_MODIFICATION_TIME(1, 1, 2020, 0, 0, 0)),
        MOCK_FILE_WITH_STATS("same_z", MOCK_MODIFICATION_TIME(1, 1, 2024, 0, 0, 0)),
        MOCK_FILE_WITH_STATS("same_a", MOCK_MODIFICATION_TIME(1, 1, 2024, 0, 0, 0)),
        MOCK_FILE_WITH_STATS("new", MOCK_MODIFICATION_TIME(1, 1, 2025, 0, 0, 0)),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-t", "old", "same_z", "same_a", "new", NULL };
    get_parsed_arguments_result(5, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(result, "new\nsame_a\nsame_z\nold\n");
}

static void should_sort_multiple_directory_operands_and_their_contents_by_modification_time(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR_WITH_STATS("dir_old", ((const char *[]){ ".", "..", "old", "new", NULL }), MOCK_MODIFICATION_TIME(1, 1, 2020, 0, 0, 0)),
        MOCK_DIR_WITH_STATS("dir_new", ((const char *[]){ ".", "..", "old", "new", NULL }), MOCK_MODIFICATION_TIME(1, 1, 2025, 0, 0, 0)),
        MOCK_FILE_WITH_STATS("dir_old/old", MOCK_MODIFICATION_TIME(1, 1, 2020, 0, 0, 0)),
        MOCK_FILE_WITH_STATS("dir_old/new", MOCK_MODIFICATION_TIME(1, 1, 2025, 0, 0, 0)),
        MOCK_FILE_WITH_STATS("dir_new/old", MOCK_MODIFICATION_TIME(1, 1, 2020, 0, 0, 0)),
        MOCK_FILE_WITH_STATS("dir_new/new", MOCK_MODIFICATION_TIME(1, 1, 2025, 0, 0, 0)),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-t", "dir_old", "dir_new", NULL };
    get_parsed_arguments_result(3, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(
        result,
        "dir_new:\n"
        "new\n"
        "old\n"
        "\n"
        "dir_old:\n"
        "new\n"
        "old\n"
    );
}

static void should_sort_mixed_file_and_directory_operands_separately_by_modification_time(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_WITH_STATS("file_old", MOCK_MODIFICATION_TIME(1, 1, 2020, 0, 0, 0)),
        MOCK_FILE_WITH_STATS("file_new", MOCK_MODIFICATION_TIME(1, 1, 2025, 0, 0, 0)),
        MOCK_DIR_WITH_STATS("dir_old", ((const char *[]){ ".", "..", "old", "new", NULL }), MOCK_MODIFICATION_TIME(1, 1, 2020, 0, 0, 0)),
        MOCK_DIR_WITH_STATS("dir_new", ((const char *[]){ ".", "..", "old", "new", NULL }), MOCK_MODIFICATION_TIME(1, 1, 2025, 0, 0, 0)),
        MOCK_FILE_WITH_STATS("dir_old/old", MOCK_MODIFICATION_TIME(1, 1, 2020, 0, 0, 0)),
        MOCK_FILE_WITH_STATS("dir_old/new", MOCK_MODIFICATION_TIME(1, 1, 2025, 0, 0, 0)),
        MOCK_FILE_WITH_STATS("dir_new/old", MOCK_MODIFICATION_TIME(1, 1, 2020, 0, 0, 0)),
        MOCK_FILE_WITH_STATS("dir_new/new", MOCK_MODIFICATION_TIME(1, 1, 2025, 0, 0, 0)),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-t", "file_old", "dir_old", "file_new", "dir_new", NULL };
    get_parsed_arguments_result(5, arguments);
    const int result = application_run(parsed_arguments_result);

    assert_application_succeeded_with_output(
        result,
        "file_new\n"
        "file_old\n"
        "\n"
        "dir_new:\n"
        "new\n"
        "old\n"
        "\n"
        "dir_old:\n"
        "new\n"
        "old\n"
    );
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

static void should_fail_with_a_major_error_and_not_print_anything_if_fails_to_open_the_current_directory_without_arguments(void)
{
    const t_vfs_mock_entry vfs[] = {
        {
            .path = ".",
            .stats = { .st_mode = S_IFDIR | 0755 },
            .entries = (const char *[]){ ".", "..", "file", NULL },
            .target = NULL,
            .errors ={
                .stat_errno = EACCES,
                .lstat_errno =EACCES,
                .opendir_errno =EACCES,
                .closedir_errno = 0
            }
        },
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { NULL };
    get_parsed_arguments_result(0, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_no_output_was_printed());
    CU_ASSERT(verify_that_the_error_printed_is("ft_ls: cannot open directory '%s': %s\n", ".", strerror(EACCES)));
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
        MOCK_FILE("workingDir/fileDir"),
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
        MOCK_FILE("workingDir/fileDir"),
        MOCK_FILE("zWorkingDir/zFileDir"),
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
        MOCK_FILE("zdir/file1"),
        MOCK_DIR("zdir/subdir1", ".", ".."),
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
        MOCK_FILE("zdir/file1"),
        MOCK_DIR("zdir/subdir1", ".", ".."),
        MOCK_FILE("workingDir/fileDir"),
        MOCK_FILE("zWorkingDir/zFileDir"),
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

static void should_fail_with_a_major_error_and_not_print_anything_if_fails_to_access_the_current_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR_ACCESS_ERROR(EACCES, ".", "..", ".", "file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { ".", NULL };
    get_parsed_arguments_result(1, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_no_output_was_printed());
    CU_ASSERT(verify_that_the_error_printed_is("ft_ls: cannot access '%s': %s\n", ".", strerror(EACCES)));
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
        MOCK_FILE("dir/file"),
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
        MOCK_FILE("workingDir/fileDir"),
        MOCK_FILE("zWorkingDir/zFileDir"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "linkloop", "file", "workingDir", "zWorkingDir", "zFile", NULL };
    const char *expected_file_names[] = { "file", "zFile" };
    const char *expected_first_dir_file_name = "fileDir";
    const char *expected_second_dir_file_name = "zFileDir";
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
        MOCK_FILE("workingDir/fileDir"),
        MOCK_FILE("zWorkingDir/zFileDir"),
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

static void should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_access_a_child_file_of_a_directory_operand(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file", "subdir", "file2", "subdir2", "symlink"),
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_SYMLINK("symlink", "subdir"),
        MOCK_DIR("subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_DIR("subdir2", ".", "..", "sbd_dir2"),
        MOCK_DIR("./subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_DIR("./subdir2", ".", "..", "sbd_dir2"),
        MOCK_FILE_ACCESS_ERROR(ENOTDIR, "./subdir/sbd_file1"),
        MOCK_DIR("./subdir/sbd_dir1", ".", ".."),
        MOCK_DIR("./subdir2/sbd_dir2", ".", "..", "char", "block"),
        MOCK_CHAR_DEVICE("./subdir2/sbd_dir2/char"),
        MOCK_BLOCK_DEVICE("./subdir2/sbd_dir2/block"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-R", NULL };
    const char *expected_file_names[] = { "file", "file2", "symlink", "sbd_file1", "block", "char" };
    const char *expected_dir_headers[] = { ".", "./subdir", "./subdir/sbd_dir1", "./subdir2", "./subdir2/sbd_dir2" };
    const char *expected_subdirs[] = { "subdir", "subdir2", "sbd_dir1", "sbd_dir2" };
    get_parsed_arguments_result(1, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s:\n"
        "%s\n%s\n%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "\n%s:\n"
        "%s\n"
        "\n%s:\n"
        "%s\n%s\n",
        expected_dir_headers[0],
        expected_file_names[0],
        expected_file_names[1],
        expected_subdirs[0],
        expected_subdirs[1],
        expected_file_names[2],
        expected_dir_headers[1],
        expected_subdirs[2],
        expected_file_names[3],
        expected_dir_headers[2],
        expected_dir_headers[3],
        expected_subdirs[3],
        expected_dir_headers[4],
        expected_file_names[4],
        expected_file_names[5]
    ));
    CU_ASSERT(verify_that_the_error_printed_is("ft_ls: cannot access '%s': %s\n", "./subdir/sbd_file1", strerror(ENOTDIR)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MINOR_ERROR);
}

static void should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_open_a_subdirectory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "..", "file", "subdir", "file2", "subdir2", "symlink"),
        MOCK_FILE("dir/file"),
        MOCK_FILE("dir/file2"),
        MOCK_SYMLINK("dir/symlink", "dir/subdir"),
        MOCK_DIR("subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_DIR("subdir2", ".", "..", "sbd_dir2"),
        MOCK_DIR("dir/subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_DIR("dir/subdir2", ".", "..", "sbd_dir2"),
        MOCK_FILE("dir/subdir/sbd_file1"),
        MOCK_DIR_OPEN_ERROR(EACCES, "dir/subdir/sbd_dir1", ".", ".."),
        MOCK_DIR("dir/subdir2/sbd_dir2", ".", "..", "char", "block"),
        MOCK_CHAR_DEVICE("dir/subdir2/sbd_dir2/char"),
        MOCK_BLOCK_DEVICE("dir/subdir2/sbd_dir2/block"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-R", "dir", NULL };
    const char *expected_file_names[] = { "file", "file2", "symlink", "sbd_file1", "block", "char" };
    const char *expected_dir_headers[] = { "dir", "dir/subdir", "dir/subdir2", "dir/subdir2/sbd_dir2" };
    const char *expected_subdirs[] = { "subdir", "subdir2", "sbd_dir1", "sbd_dir2" };
    get_parsed_arguments_result(2, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s:\n"
        "%s\n%s\n%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n"
        "\n%s:\n"
        "%s\n%s\n",
        expected_dir_headers[0],
        expected_file_names[0],
        expected_file_names[1],
        expected_subdirs[0],
        expected_subdirs[1],
        expected_file_names[2],
        expected_dir_headers[1],
        expected_subdirs[2],
        expected_file_names[3],
        expected_dir_headers[2],
        expected_subdirs[3],
        expected_dir_headers[3],
        expected_file_names[4],
        expected_file_names[5]
    ));
    CU_ASSERT(verify_that_the_error_printed_is("ft_ls: cannot open directory '%s': %s\n", "dir/subdir/sbd_dir1", strerror(EACCES)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MINOR_ERROR);
}

static void should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_read_a_subdirectory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "..", "file", "subdir", "file2", "subdir2", "symlink"),
        MOCK_FILE("dir/file"),
        MOCK_FILE("dir/file2"),
        MOCK_SYMLINK("dir/symlink", "dir/subdir"),
        MOCK_DIR("subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_DIR("subdir2", ".", "..", "sbd_dir2"),
        MOCK_DIR("dir/subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_DIR("dir/subdir2", ".", "..", "sbd_dir2"),
        MOCK_FILE("dir/subdir/sbd_file1"),
        MOCK_DIR_READ_ERROR("dir/subdir/sbd_dir1", 4, ".", "..", "failedFile1", "failedDir1", "failedFile2", "failedDir2"),
        MOCK_FILE("dir/subdir/sbd_dir1/failedFile1"),
        MOCK_DIR("dir/subdir/sbd_dir1/failedDir1", ".", "..", "sock", "fifo"),
        MOCK_FILE("dir/subdir/sbd_dir1/failedDir1/sock"),
        MOCK_FILE("dir/subdir/sbd_dir1/failedDir1/fifo"),
        MOCK_FILE("dir/subdir/sbd_dir1/failedFile2"),
        MOCK_DIR("dir/subdir/sbd_dir1/failedDir2", ".", ".."),
        MOCK_FILE("dir/subdir/sbd_file1"),
        MOCK_DIR("dir/subdir2/sbd_dir2", ".", "..", "char", "block"),
        MOCK_CHAR_DEVICE("dir/subdir2/sbd_dir2/char"),
        MOCK_BLOCK_DEVICE("dir/subdir2/sbd_dir2/block"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "dir", "-R", NULL };
    const char *expected_file_names[] = { "file", "file2", "symlink", "sbd_file1", "failedFile1", "fifo", "sock", "block", "char" };
    const char *expected_dir_headers[] = { "dir", "dir/subdir", "dir/subdir/sbd_dir1", "dir/subdir/sbd_dir1/failedDir1", "dir/subdir2", "dir/subdir2/sbd_dir2" };
    const char *expected_subdirs[] = { "subdir", "subdir2", "sbd_dir1", "failedDir1", "sbd_dir2" };
    get_parsed_arguments_result(2, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s:\n"
        "%s\n%s\n%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n"
        "\n%s:\n"
        "%s\n%s\n",
        expected_dir_headers[0],
        expected_file_names[0],
        expected_file_names[1],
        expected_subdirs[0],
        expected_subdirs[1],
        expected_file_names[2],
        expected_dir_headers[1],
        expected_subdirs[2],
        expected_file_names[3],
        expected_dir_headers[2],
        expected_subdirs[3],
        expected_file_names[4],
        expected_dir_headers[3],
        expected_file_names[5],
        expected_file_names[6],
        expected_dir_headers[4],
        expected_subdirs[4],
        expected_dir_headers[5],
        expected_file_names[7],
        expected_file_names[8]
    ));
    CU_ASSERT(verify_that_the_error_printed_is("ft_ls: reading directory '%s': %s\n", "dir/subdir/sbd_dir1", strerror(EBADF)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MINOR_ERROR);
}

static void should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_close_a_subdirectory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file", "subdir", "file2", "subdir2"),
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_DIR("subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_DIR("subdir2", ".", "..", "sbd_dir2"),
        MOCK_FILE("subdir/sbd_file1"),
        MOCK_DIR_CLOSE_ERROR("subdir/sbd_dir1", ".", "..", "failedFile1", "failedDir1", "failedFile2", "failedDir2"),
        MOCK_FILE("subdir/sbd_dir1/failedFile1"),
        MOCK_DIR("subdir/sbd_dir1/failedDir1", ".", "..", "sock", "fifo"),
        MOCK_FILE("subdir/sbd_dir1/failedDir1/sock"),
        MOCK_FILE("subdir/sbd_dir1/failedDir1/fifo"),
        MOCK_FILE("subdir/sbd_dir1/failedFile2"),
        MOCK_DIR("subdir/sbd_dir1/failedDir2", ".", ".."),
        MOCK_FILE("subdir/sbd_file1"),
        MOCK_DIR("subdir2/sbd_dir2", ".", "..", "char", "block"),
        MOCK_CHAR_DEVICE("subdir2/sbd_dir2/char"),
        MOCK_BLOCK_DEVICE("subdir2/sbd_dir2/block"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "file", "subdir", "file2", "subdir2", "-R", NULL };
    const char *expected_file_names[] = { "file", "file2", "sbd_file1", "failedFile1", "failedFile2", "fifo", "sock", "block", "char" };
    const char *expected_dir_headers[] = { "subdir", "subdir/sbd_dir1", "subdir/sbd_dir1/failedDir1", "subdir/sbd_dir1/failedDir2", "subdir2", "subdir2/sbd_dir2" };
    const char *expected_subdirs[] = { "subdir", "subdir2", "sbd_dir1", "failedDir1", "failedDir2", "sbd_dir2" };
    get_parsed_arguments_result(5, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "\n%s:\n"
        "%s\n"
        "\n%s:\n"
        "%s\n%s\n",
        expected_file_names[0],
        expected_file_names[1],
        expected_dir_headers[0],
        expected_subdirs[2],
        expected_file_names[2],
        expected_dir_headers[1],
        expected_subdirs[3],
        expected_subdirs[4],
        expected_file_names[3],
        expected_file_names[4],
        expected_dir_headers[2],
        expected_file_names[5],
        expected_file_names[6],
        expected_dir_headers[3],
        expected_dir_headers[4],
        expected_subdirs[5],
        expected_dir_headers[5],
        expected_file_names[7],
        expected_file_names[8]
    ));
    CU_ASSERT(verify_that_the_error_printed_is("ft_ls: closing directory '%s': %s\n", "subdir/sbd_dir1", strerror(EBADF)));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MINOR_ERROR);
}

static void should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_access_content_of_a_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file", "subdir", "file2", "subdir2"),
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_DIR("subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_DIR("subdir2", ".", "..", "sbd_dir2"),
        MOCK_FILE("subdir/sbd_file1"),
        MOCK_DIR("subdir/sbd_dir1", ".", "..", "sbdFile1", "sbdDir1", "sbdFile2", "sbdDir2"),
        MOCK_FILE_ACCESS_ERROR(ENAMETOOLONG, "subdir/sbd_dir1/sbdFile1"),
        MOCK_DIR_ACCESS_ERROR(ENOTDIR, "subdir/sbd_dir1/sbdDir1", ".", "..", "sock", "fifo"),
        MOCK_FILE("subdir/sbd_dir1/sbdDir1/sock"),
        MOCK_FILE("subdir/sbd_dir1/sbdDir1/fifo"),
        MOCK_FILE("subdir/sbd_dir1/sbdFile2"),
        MOCK_DIR("subdir/sbd_dir1/sbdDir2", ".", ".."),
        MOCK_DIR("subdir2/sbd_dir2", ".", "..", "char", "block"),
        MOCK_CHAR_DEVICE("subdir2/sbd_dir2/char"),
        MOCK_BLOCK_DEVICE("subdir2/sbd_dir2/block"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-R", "file", "subdir", "file2", "subdir2", NULL };
    const char *expected_non_dir_operands[] = { "file", "file2" };
    const char *expected_dir_headers[] = { "subdir", "subdir/sbd_dir1", "subdir/sbd_dir1/sbdDir2", "subdir2", "subdir2/sbd_dir2" };
    const char *expected_subdir_content[] = { "sbd_dir1", "sbd_file1" };
    const char *expected_subdir2_content[] = { "sbd_dir2" };
    const char *expected_sbdDir1_content[] = { "sbdDir1", "sbdDir2", "sbdFile1", "sbdFile2" };
    const char *expected_sbdDir2_content[] = { "block", "char" };
    get_parsed_arguments_result(5, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n%s\n%s\n"
        "\n%s:\n"
        "\n%s:\n"
        "%s\n"
        "\n%s:\n"
        "%s\n%s\n",
        expected_non_dir_operands[0],
        expected_non_dir_operands[1],
        expected_dir_headers[0],
        expected_subdir_content[0],
        expected_subdir_content[1],
        expected_dir_headers[1],
        expected_sbdDir1_content[0],
        expected_sbdDir1_content[1],
        expected_sbdDir1_content[2],
        expected_sbdDir1_content[3],
        expected_dir_headers[2],
        expected_dir_headers[3],
        expected_subdir2_content[0],
        expected_dir_headers[4],
        expected_sbdDir2_content[0],
        expected_sbdDir2_content[1]
    ));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot access '%s': %s\n",
        "subdir/sbd_dir1/sbdFile1", strerror(ENAMETOOLONG),
        "subdir/sbd_dir1/sbdDir1", strerror(ENOTDIR)
    ));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MINOR_ERROR);
}

static void should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_access_content_of_different_directories(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file", "subdir", "file2", "subdir2"),
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_DIR("subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_DIR("subdir2", ".", "..", "sbd_dir2"),
        MOCK_FILE("subdir/sbd_file1"),
        MOCK_DIR("subdir/sbd_dir1", ".", "..", "sbdFile1", "sbdDir1", "sbdFile2", "sbdDir2"),
        MOCK_FILE("subdir/sbd_dir1/sbdFile1"),
        MOCK_DIR_ACCESS_ERROR(ENOTDIR, "subdir/sbd_dir1/sbdDir1", ".", "..", "sock", "fifo"),
        MOCK_FILE("subdir/sbd_dir1/sbdDir1/sock"),
        MOCK_FILE("subdir/sbd_dir1/sbdDir1/fifo"),
        MOCK_FILE("subdir/sbd_dir1/sbdFile2"),
        MOCK_DIR("subdir/sbd_dir1/sbdDir2", ".", ".."),
        MOCK_DIR_ACCESS_ERROR(ENAMETOOLONG, "subdir2/sbd_dir2", ".", "..", "char", "block"),
        MOCK_CHAR_DEVICE("subdir2/sbd_dir2/char"),
        MOCK_BLOCK_DEVICE("subdir2/sbd_dir2/block"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-R", "file", "subdir", "file2", "subdir2", NULL };
    const char *expected_non_dir_operands[] = { "file", "file2" };
    const char *expected_dir_headers[] = { "subdir", "subdir/sbd_dir1", "subdir/sbd_dir1/sbdDir2", "subdir2" };
    const char *expected_subdir_content[] = { "sbd_dir1", "sbd_file1" };
    const char *expected_subdir2_content[] = { "sbd_dir2" };
    const char *expected_sbdDir1_content[] = { "sbdDir1", "sbdDir2", "sbdFile1", "sbdFile2" };
    get_parsed_arguments_result(5, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n%s\n%s\n"
        "\n%s:\n"
        "\n%s:\n"
        "%s\n",
        expected_non_dir_operands[0],
        expected_non_dir_operands[1],
        expected_dir_headers[0],
        expected_subdir_content[0],
        expected_subdir_content[1],
        expected_dir_headers[1],
        expected_sbdDir1_content[0],
        expected_sbdDir1_content[1],
        expected_sbdDir1_content[2],
        expected_sbdDir1_content[3],
        expected_dir_headers[2],
        expected_dir_headers[3],
        expected_subdir2_content[0]
    ));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot access '%s': %s\n",
        "subdir/sbd_dir1/sbdDir1", strerror(ENOTDIR),
        "subdir2/sbd_dir2", strerror(ENAMETOOLONG)
    ));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MINOR_ERROR);
}

static void should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_access_and_open_content_of_different_directories(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file", "subdir", "file2", "subdir2"),
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_DIR("subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_DIR("subdir2", ".", "..", "sbd_dir2"),
        MOCK_FILE("subdir/sbd_file1"),
        MOCK_DIR("subdir/sbd_dir1", ".", "..", "sbdFile1", "sbdDir1", "sbdFile2", "sbdDir2"),
        MOCK_FILE("subdir/sbd_dir1/sbdFile1"),
        MOCK_DIR_ACCESS_ERROR(ENOTDIR, "subdir/sbd_dir1/sbdDir1", ".", "..", "sock", "fifo"),
        MOCK_FILE("subdir/sbd_dir1/sbdDir1/sock"),
        MOCK_FILE("subdir/sbd_dir1/sbdDir1/fifo"),
        MOCK_FILE("subdir/sbd_dir1/sbdFile2"),
        MOCK_DIR("subdir/sbd_dir1/sbdDir2", ".", ".."),
        MOCK_DIR_OPEN_ERROR(EACCES, "subdir2/sbd_dir2", ".", "..", "char", "block"),
        MOCK_CHAR_DEVICE("subdir2/sbd_dir2/char"),
        MOCK_BLOCK_DEVICE("subdir2/sbd_dir2/block"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-R", "file", "subdir", "file2", "subdir2", NULL };
    const char *expected_non_dir_operands[] = { "file", "file2" };
    const char *expected_dir_headers[] = { "subdir", "subdir/sbd_dir1", "subdir/sbd_dir1/sbdDir2", "subdir2" };
    const char *expected_subdir_content[] = { "sbd_dir1", "sbd_file1" };
    const char *expected_subdir2_content[] = { "sbd_dir2" };
    const char *expected_sbdDir1_content[] = { "sbdDir1", "sbdDir2", "sbdFile1", "sbdFile2" };
    get_parsed_arguments_result(5, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n%s\n%s\n"
        "\n%s:\n"
        "\n%s:\n"
        "%s\n",
        expected_non_dir_operands[0],
        expected_non_dir_operands[1],
        expected_dir_headers[0],
        expected_subdir_content[0],
        expected_subdir_content[1],
        expected_dir_headers[1],
        expected_sbdDir1_content[0],
        expected_sbdDir1_content[1],
        expected_sbdDir1_content[2],
        expected_sbdDir1_content[3],
        expected_dir_headers[2],
        expected_dir_headers[3],
        expected_subdir2_content[0]
    ));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot open directory '%s': %s\n",
        "subdir/sbd_dir1/sbdDir1", strerror(ENOTDIR),
        "subdir2/sbd_dir2", strerror(EACCES)
    ));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MINOR_ERROR);
}

static void should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_access_and_read_content_of_a_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file", "subdir", "file2", "subdir2"),
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_DIR("subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_DIR("subdir2", ".", "..", "sbd_dir2"),
        MOCK_FILE("subdir/sbd_file1"),
        MOCK_DIR_READ_ERROR("subdir/sbd_dir1", 4, ".", "..", "sbdFile1", "sbdDir1", "sbdFile2", "sbdDir2"),
        MOCK_FILE_ACCESS_ERROR(ENAMETOOLONG, "subdir/sbd_dir1/sbdFile1"),
        MOCK_DIR("subdir/sbd_dir1/sbdDir1", ".", "..", "sock", "fifo"),
        MOCK_FILE("subdir/sbd_dir1/sbdDir1/sock"),
        MOCK_FILE("subdir/sbd_dir1/sbdDir1/fifo"),
        MOCK_FILE("subdir/sbd_dir1/sbdFile2"),
        MOCK_DIR("subdir/sbd_dir1/sbdDir2", ".", ".."),
        MOCK_DIR("subdir2/sbd_dir2", ".", "..", "char", "block"),
        MOCK_CHAR_DEVICE("subdir2/sbd_dir2/char"),
        MOCK_BLOCK_DEVICE("subdir2/sbd_dir2/block"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-R", "file", "subdir", "file2", "subdir2", NULL };
    const char *expected_non_dir_operands[] = { "file", "file2" };
    const char *expected_dir_headers[] = { "subdir", "subdir/sbd_dir1", "subdir/sbd_dir1/sbdDir1", "subdir2", "subdir2/sbd_dir2" };
    const char *expected_subdir_content[] = { "sbd_dir1", "sbd_file1" };
    const char *expected_subdir2_content[] = { "sbd_dir2" };
    const char *expected_sbd_dir1_content[] = { "sbdDir1", "sbdFile1" };
    const char *expected_sbdDir1_content[] = { "fifo", "sock" };
    const char *expected_sbdDir2_content[] = { "block", "char" };
    get_parsed_arguments_result(5, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n"
        "\n%s:\n"
        "%s\n%s\n",
        expected_non_dir_operands[0],
        expected_non_dir_operands[1],
        expected_dir_headers[0],
        expected_subdir_content[0],
        expected_subdir_content[1],
        expected_dir_headers[1],
        expected_sbd_dir1_content[0],
        expected_sbd_dir1_content[1],
        expected_dir_headers[2],
        expected_sbdDir1_content[0],
        expected_sbdDir1_content[1],
        expected_dir_headers[3],
        expected_subdir2_content[0],
        expected_dir_headers[4],
        expected_sbdDir2_content[0],
        expected_sbdDir2_content[1]
    ));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: reading directory '%s': %s\n",
        "subdir/sbd_dir1/sbdFile1", strerror(ENAMETOOLONG),
        "subdir/sbd_dir1", strerror(EBADF)
    ));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MINOR_ERROR);
}

static void should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_access_and_close_content_of_a_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file", "subdir", "file2", "subdir2"),
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_DIR("subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_DIR("subdir2", ".", "..", "sbd_dir2"),
        MOCK_FILE("subdir/sbd_file1"),
        MOCK_DIR_CLOSE_ERROR("subdir/sbd_dir1", ".", "..", "sbdFile1", "sbdDir1", "sbdFile2", "sbdDir2"),
        MOCK_FILE_ACCESS_ERROR(ENAMETOOLONG, "subdir/sbd_dir1/sbdFile1"),
        MOCK_DIR("subdir/sbd_dir1/sbdDir1", ".", "..", "sock", "fifo"),
        MOCK_FILE("subdir/sbd_dir1/sbdDir1/sock"),
        MOCK_FILE("subdir/sbd_dir1/sbdDir1/fifo"),
        MOCK_FILE("subdir/sbd_dir1/sbdFile2"),
        MOCK_DIR("subdir/sbd_dir1/sbdDir2", ".", ".."),
        MOCK_DIR("subdir2/sbd_dir2", ".", "..", "char", "block"),
        MOCK_CHAR_DEVICE("subdir2/sbd_dir2/char"),
        MOCK_BLOCK_DEVICE("subdir2/sbd_dir2/block"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-R", "file", "subdir", "file2", "subdir2", NULL };
    const char *expected_non_dir_operands[] = { "file", "file2" };
    const char *expected_dir_headers[] = { "subdir", "subdir/sbd_dir1", "subdir/sbd_dir1/sbdDir1", "subdir/sbd_dir1/sbdDir2", "subdir2", "subdir2/sbd_dir2" };
    const char *expected_subdir_content[] = { "sbd_dir1", "sbd_file1" };
    const char *expected_subdir2_content[] = { "sbd_dir2" };
    const char *expected_sbd_dir1_content[] = { "sbdDir1", "sbdDir2", "sbdFile1", "sbdFile2" };
    const char *expected_sbd_dir2_content[] = { "block", "char" };
    const char *expected_sbdDir1_content[] = { "fifo", "sock" };
    get_parsed_arguments_result(5, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "\n%s:\n"
        "%s\n"
        "\n%s:\n"
        "%s\n%s\n",
        expected_non_dir_operands[0],
        expected_non_dir_operands[1],
        expected_dir_headers[0],
        expected_subdir_content[0],
        expected_subdir_content[1],
        expected_dir_headers[1],
        expected_sbd_dir1_content[0],
        expected_sbd_dir1_content[1],
        expected_sbd_dir1_content[2],
        expected_sbd_dir1_content[3],
        expected_dir_headers[2],
        expected_sbdDir1_content[0],
        expected_sbdDir1_content[1],
        expected_dir_headers[3],
        expected_dir_headers[4],
        expected_subdir2_content[0],
        expected_dir_headers[5],
        expected_sbd_dir2_content[0],
        expected_sbd_dir2_content[1]
    ));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: closing directory '%s': %s\n",
        "subdir/sbd_dir1/sbdFile1", strerror(ENAMETOOLONG),
        "subdir/sbd_dir1", strerror(EBADF)
    ));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MINOR_ERROR);
}

static void should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_operate_multiple_subdirectories(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file", "subdir", "file2", "subdir2"),
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_DIR("subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_DIR("subdir2", ".", "..", "sbd_dir2"),
        MOCK_FILE("subdir/sbd_file1"),
        MOCK_DIR_OPEN_ERROR(EACCES, "subdir/sbd_dir1", ".", "..", "sbdFile1", "sbdDir1", "sbdFile2", "sbdDir2"),
        MOCK_DIR_OPEN_ERROR(EACCES, "subdir2/sbd_dir2", ".", "..", "char", "block"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-R", "file", "subdir", "file2", "subdir2", NULL };
    const char *expected_non_dir_operands[] = { "file", "file2" };
    const char *expected_dir_headers[] = { "subdir", "subdir2" };
    const char *expected_subdir_content[] = { "sbd_dir1", "sbd_file1" };
    const char *expected_subdir2_content[] = { "sbd_dir2" };
    get_parsed_arguments_result(5, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n",
        expected_non_dir_operands[0],
        expected_non_dir_operands[1],
        expected_dir_headers[0],
        expected_subdir_content[0],
        expected_subdir_content[1],
        expected_dir_headers[1],
        expected_subdir2_content[0]
    ));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot open directory '%s': %s\n"
        "ft_ls: cannot open directory '%s': %s\n",
        "subdir/sbd_dir1", strerror(EACCES),
        "subdir2/sbd_dir2", strerror(EACCES)
    ));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MINOR_ERROR);
}

static void should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_access_subdir_content_and_to_open_directory_operand(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file", "subdir", "file2", "subdir2"),
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_DIR("subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_FILE("subdir/sbd_file1"),
        MOCK_DIR_ACCESS_ERROR(ENOTDIR, "subdir/sbd_dir1", ".", ".."),
        MOCK_DIR_OPEN_ERROR(EACCES, "subdir2", ".", "..", "sbd_dir2"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-R", "file", "subdir", "file2", "subdir2", NULL };
    const char *expected_non_dir_operands[] = { "file", "file2" };
    const char *expected_dir_headers[] = { "subdir" };
    const char *expected_subdir_content[] = { "sbd_dir1", "sbd_file1" };
    get_parsed_arguments_result(5, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n",
        expected_non_dir_operands[0],
        expected_non_dir_operands[1],
        expected_dir_headers[0],
        expected_subdir_content[0],
        expected_subdir_content[1]
    ));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot open directory '%s': %s\n",
        "subdir/sbd_dir1", strerror(ENOTDIR),
        "subdir2", strerror(EACCES)
    ));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_read_subdir_content_and_directory_operand(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file", "subdir", "file2", "subdir2"),
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_DIR("subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_FILE("subdir/sbd_file1"),
        MOCK_DIR_READ_ERROR("subdir/sbd_dir1", 3, ".", "..", "sbdFile1", "sbdDir1", "sbdFile2", "sbdDir2"),
        MOCK_FILE("subdir/sbd_dir1/sbdFile1"),
        MOCK_DIR_READ_ERROR("subdir2", 2, ".", "..", "sbd_dir2", "sbd_file2"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-R", "file", "subdir", "file2", "subdir2", NULL };
    const char *expected_non_dir_operands[] = { "file", "file2" };
    const char *expected_dir_headers[] = { "subdir", "subdir/sbd_dir1", "subdir2" };
    const char *expected_subdir_content[] = { "sbd_dir1", "sbd_file1" };
    const char *expected_sbd_dir1_content[] = { "sbdFile1" };
    get_parsed_arguments_result(5, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n"
        "\n%s:\n",
        expected_non_dir_operands[0],
        expected_non_dir_operands[1],
        expected_dir_headers[0],
        expected_subdir_content[0],
        expected_subdir_content[1],
        expected_dir_headers[1],
        expected_sbd_dir1_content[0],
        expected_dir_headers[2]
    ));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: reading directory '%s': %s\n"
        "ft_ls: reading directory '%s': %s\n",
        "subdir/sbd_dir1", strerror(EBADF),
        "subdir2", strerror(EBADF)
    ));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_dir_operand_content_and_read_directory_operand(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file", "subdir", "file2"),
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_DIR_READ_ERROR("subdir", 3, ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_FILE_ACCESS_ERROR(ENOTDIR, "subdir/sbd_file1"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-R", "file", "subdir", "file2", NULL };
    const char *expected_non_dir_operands[] = { "file", "file2" };
    const char *expected_dir_headers[] = { "subdir" };
    const char *expected_subdir_content[] = { "sbd_file1" };
    get_parsed_arguments_result(4, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n",
        expected_non_dir_operands[0],
        expected_non_dir_operands[1],
        expected_dir_headers[0],
        expected_subdir_content[0]
    ));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: reading directory '%s': %s\n",
        "subdir/sbd_file1", strerror(ENOTDIR),
        "subdir", strerror(EBADF)
    ));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_open_subdir_content_and_close_operand(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file", "subdir", "file2", "subdir2"),
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_DIR("subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_FILE("subdir/sbd_file1"),
        MOCK_DIR_OPEN_ERROR(EACCES, "subdir/sbd_dir1", ".", ".."),
        MOCK_DIR_CLOSE_ERROR("subdir2", ".", "..", "sbd_dir2", "sbd_file2"),
        MOCK_DIR("subdir2/sbd_dir2", ".", "..", "symlink"),
        MOCK_SYMLINK("subdir2/sbd_dir2/symlink", "subdir"),
        MOCK_FILE("subdir2/sbd_file2"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-R", "file", "subdir", "file2", "subdir2", NULL };
    const char *expected_non_dir_operands[] = { "file", "file2" };
    const char *expected_dir_headers[] = { "subdir", "subdir2", "subdir2/sbd_dir2" };
    const char *expected_subdir_content[] = { "sbd_dir1", "sbd_file1" };
    const char *expected_subdir2_content[] = { "sbd_dir2", "sbd_file2" };
    const char *expected_sbd_dir2_content[] = { "symlink" };
    get_parsed_arguments_result(5, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n",
        expected_non_dir_operands[0],
        expected_non_dir_operands[1],
        expected_dir_headers[0],
        expected_subdir_content[0],
        expected_subdir_content[1],
        expected_dir_headers[1],
        expected_subdir2_content[0],
        expected_subdir2_content[1],
        expected_dir_headers[2],
        expected_sbd_dir2_content[0]
    ));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot open directory '%s': %s\n"
        "ft_ls: closing directory '%s': %s\n",
        "subdir/sbd_dir1", strerror(EACCES),
        "subdir2", strerror(EBADF)
    ));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_access_an_operand_and_open_subdirectory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file", "subdir", "file2", "subdir2"),
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_DIR_ACCESS_ERROR(ENOTDIR, "subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_DIR("subdir2", ".", "..", "sbd_dir2", "sbd_file2"),
        MOCK_DIR("subdir2/sbd_dir2", ".", "..", "symlink"),
        MOCK_SYMLINK("subdir2/sbd_dir2/symlink", "subdir"),
        MOCK_DIR_OPEN_ERROR(EACCES, "subdir2/sbd_file2", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-R", "file", "subdir", "file2", "subdir2", NULL };
    const char *expected_non_dir_operands[] = { "file", "file2" };
    const char *expected_dir_headers[] = { "subdir2", "subdir2/sbd_dir2" };
    const char *expected_subdir2_content[] = { "sbd_dir2", "sbd_file2" };
    const char *expected_sbd_dir2_content[] = { "symlink" };
    get_parsed_arguments_result(5, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n",
        expected_non_dir_operands[0],
        expected_non_dir_operands[1],
        expected_dir_headers[0],
        expected_subdir2_content[0],
        expected_subdir2_content[1],
        expected_dir_headers[1],
        expected_sbd_dir2_content[0]
    ));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot open directory '%s': %s\n",
        "subdir", strerror(ENOTDIR),
        "subdir2/sbd_file2", strerror(EACCES)
    ));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_open_a_directory_operand_and_to_read_subdirectory_content(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file", "subdir", "file2", "subdir2"),
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_DIR_OPEN_ERROR(EACCES, "subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_DIR("subdir2", ".", "..", "sbd_dir2", "sbd_file2"),
        MOCK_DIR_READ_ERROR("subdir2/sbd_dir2", 3, ".", "..", "finalFile", "finalDir"),
        MOCK_FILE("subdir2/sbd_dir2/finalFile"),
        MOCK_FILE("subdir2/sbd_file2"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-R", "file", "subdir", "file2", "subdir2", NULL };
    const char *expected_non_dir_operands[] = { "file", "file2" };
    const char *expected_dir_headers[] = { "subdir2", "subdir2/sbd_dir2" };
    const char *expected_subdir2_content[] = { "sbd_dir2", "sbd_file2" };
    const char *expected_sbd_dir2_content[] = { "finalFile" };
    get_parsed_arguments_result(5, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n",
        expected_non_dir_operands[0],
        expected_non_dir_operands[1],
        expected_dir_headers[0],
        expected_subdir2_content[0],
        expected_subdir2_content[1],
        expected_dir_headers[1],
        expected_sbd_dir2_content[0]
    ));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot open directory '%s': %s\n"
        "ft_ls: reading directory '%s': %s\n",
        "subdir", strerror(EACCES),
        "subdir2/sbd_dir2", strerror(EBADF)
    ));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_read_a_directory_operand_and_to_close_subdirectory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file", "subdir", "file2", "subdir2"),
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_DIR_READ_ERROR("subdir", 3, ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_FILE("subdir/sbd_file1"),
        MOCK_DIR("subdir2", ".", "..", "sbd_dir2", "sbd_file2"),
        MOCK_DIR_CLOSE_ERROR("subdir2/sbd_dir2", ".", "..", "finalFile", "finalDir"),
        MOCK_FILE("subdir2/sbd_dir2/finalFile"),
        MOCK_DIR("subdir2/sbd_dir2/finalDir", ".", ".."),
        MOCK_FILE("subdir2/sbd_file2"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-R", "file", "subdir", "file2", "subdir2", NULL };
    const char *expected_non_dir_operands[] = { "file", "file2" };
    const char *expected_dir_headers[] = { "subdir", "subdir2", "subdir2/sbd_dir2", "subdir2/sbd_dir2/finalDir" };
    const char *expected_subdir_content[] = { "sbd_file1" };
    const char *expected_subdir2_content[] = { "sbd_dir2", "sbd_file2" };
    const char *expected_sbd_dir2_content[] = { "finalDir", "finalFile" };
    get_parsed_arguments_result(5, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n",
        expected_non_dir_operands[0],
        expected_non_dir_operands[1],
        expected_dir_headers[0],
        expected_subdir_content[0],
        expected_dir_headers[1],
        expected_subdir2_content[0],
        expected_subdir2_content[1],
        expected_dir_headers[2],
        expected_sbd_dir2_content[0],
        expected_sbd_dir2_content[1],
        expected_dir_headers[3]
    ));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: reading directory '%s': %s\n"
        "ft_ls: closing directory '%s': %s\n",
        "subdir", strerror(EBADF),
        "subdir2/sbd_dir2", strerror(EBADF)
    ));
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_MAJOR_ERROR);
}

static void should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_close_a_directory_operand_and_to_access_subdirectory_contents(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", "..", "file", "subdir", "file2", "subdir2"),
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_DIR_CLOSE_ERROR("subdir", ".", "..", "sbd_file1", "sbd_dir1"),
        MOCK_FILE("subdir/sbd_file1"),
        MOCK_DIR("subdir/sbd_dir1", ".", "..", "file1"),
        MOCK_FILE("subdir/sbd_dir1/file1"),
        MOCK_DIR("subdir2", ".", "..", "sbd_dir2", "sbd_file2"),
        MOCK_DIR_ACCESS_ERROR(ENOTDIR, "subdir2/sbd_dir2", ".", "..", "finalFile", "finalDir"),
        MOCK_FILE("subdir2/sbd_file2"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-R", "file", "subdir", "file2", "subdir2", NULL };
    const char *expected_non_dir_operands[] = { "file", "file2" };
    const char *expected_dir_headers[] = { "subdir", "subdir/sbd_dir1", "subdir2" };
    const char *expected_subdir_content[] = { "sbd_dir1", "sbd_file1" };
    const char *expected_sbd_dir1_content[] = { "file1" };
    const char *expected_subdir2_content[] = { "sbd_dir2", "sbd_file2" };
    get_parsed_arguments_result(5, arguments);

    const int result = application_run(parsed_arguments_result);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n"
        "\n%s:\n"
        "%s\n%s\n",
        expected_non_dir_operands[0],
        expected_non_dir_operands[1],
        expected_dir_headers[0],
        expected_subdir_content[0],
        expected_subdir_content[1],
        expected_dir_headers[1],
        expected_sbd_dir1_content[0],
        expected_dir_headers[2],
        expected_subdir2_content[0],
        expected_subdir2_content[1]
    ));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: closing directory '%s': %s\n"
        "ft_ls: cannot access '%s': %s\n",
        "subdir", strerror(EBADF),
        "subdir2/sbd_dir2", strerror(ENOTDIR)
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
        CU_add_test(suite, "should_successfully_print_the_contents_of_the_current_directory", should_successfully_print_the_contents_of_the_current_directory);
        CU_add_test(suite, "should_successfully_print_the_file_name_if_a_regular_file_operand_is_specified", should_successfully_print_the_file_name_if_a_regular_file_operand_is_specified);
        CU_add_test(suite, "should_successfully_print_the_contents_of_the_directory_specified_as_an_operand", should_successfully_print_the_contents_of_the_directory_specified_as_an_operand);
        CU_add_test(suite, "should_successfully_print_directory_contents_in_reverse_order_when_reverse_option_is_active", should_successfully_print_directory_contents_in_reverse_order_when_reverse_option_is_active);
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
        CU_add_test(suite, "should_successfully_print_the_contents_of_the_symlinks_to_different_file_types", should_successfully_print_the_contents_of_the_symlinks_to_different_file_types);
        CU_add_test(suite, "should_successfully_print_the_contents_of_the_current_directory_recursively", should_successfully_print_the_contents_of_the_current_directory_recursively);
        CU_add_test(suite, "should_successfully_print_the_contents_of_the_specified_non_directory_even_with_recursive_option", should_successfully_print_the_contents_of_the_specified_non_directory_even_with_recursive_option);
        CU_add_test(suite, "should_successfully_print_the_contents_of_the_specified_directory_recursively", should_successfully_print_the_contents_of_the_specified_directory_recursively);
        CU_add_test(suite, "should_successfully_print_the_contents_of_the_specified_operands_recursively", should_successfully_print_the_contents_of_the_specified_operands_recursively);
        CU_add_test(suite, "should_render_the_default_directory_in_long_format", should_render_the_default_directory_in_long_format);
        CU_add_test(suite, "should_render_a_non_directory_operand_in_long_format", should_render_a_non_directory_operand_in_long_format);
        CU_add_test(suite, "should_render_a_symlink_to_a_directory_operand_as_a_link_in_long_format", should_render_a_symlink_to_a_directory_operand_as_a_link_in_long_format);
        CU_add_test(suite, "should_render_a_directory_operand_in_long_format", should_render_a_directory_operand_in_long_format);
        CU_add_test(suite, "should_render_mixed_operands_in_long_format", should_render_mixed_operands_in_long_format);
        CU_add_test(suite, "should_render_file_stats_for_non_directory_and_directory_entries_in_long_format", should_render_file_stats_for_non_directory_and_directory_entries_in_long_format);
        CU_add_test(suite, "should_fall_back_to_numeric_author_and_group_in_long_format", should_fall_back_to_numeric_author_and_group_in_long_format);
        CU_add_test(suite, "should_report_a_minor_error_when_lstat_fails_for_directory_content", should_report_a_minor_error_when_lstat_fails_for_directory_content);
        CU_add_test(suite, "should_report_a_minor_error_when_readlink_fails_for_directory_content", should_report_a_minor_error_when_readlink_fails_for_directory_content);
        CU_add_test(suite, "should_report_a_major_error_when_stat_fails_for_an_operand_in_long_format", should_report_a_major_error_when_stat_fails_for_an_operand_in_long_format);
        CU_add_test(suite, "should_report_a_major_error_when_readlink_fails_for_an_operand_in_long_format", should_report_a_major_error_when_readlink_fails_for_an_operand_in_long_format);
        CU_add_test(suite, "should_report_multiple_minor_errors_for_lstat_failures_in_one_directory", should_report_multiple_minor_errors_for_lstat_failures_in_one_directory);
        CU_add_test(suite, "should_report_multiple_minor_errors_for_lstat_and_readlink_failures", should_report_multiple_minor_errors_for_lstat_and_readlink_failures);
        CU_add_test(suite, "should_report_multiple_major_errors_for_stat_failures_on_operands", should_report_multiple_major_errors_for_stat_failures_on_operands);
        CU_add_test(suite, "should_report_multiple_major_errors_for_stat_and_readlink_failures_on_operands", should_report_multiple_major_errors_for_stat_and_readlink_failures_on_operands);
        CU_add_test(suite, "should_report_major_and_minor_errors_for_operands_and_directory_content", should_report_major_and_minor_errors_for_operands_and_directory_content);
        CU_add_test(suite, "should_render_broken_looped_and_multihop_links_in_long_format", should_render_broken_looped_and_multihop_links_in_long_format);
        CU_add_test(suite, "should_print_visible_and_hidden_entries_in_the_current_directory_with_read_all", should_print_visible_and_hidden_entries_in_the_current_directory_with_read_all);
        CU_add_test(suite, "should_print_visible_entries_in_the_current_directory_without_hidden_entries_with_read_all", should_print_visible_entries_in_the_current_directory_without_hidden_entries_with_read_all);
        CU_add_test(suite, "should_print_only_dot_entries_for_an_empty_current_directory_with_read_all", should_print_only_dot_entries_for_an_empty_current_directory_with_read_all);
        CU_add_test(suite, "should_print_hidden_entries_in_a_current_directory_with_only_hidden_entries_with_read_all", should_print_hidden_entries_in_a_current_directory_with_only_hidden_entries_with_read_all);
        CU_add_test(suite, "should_print_visible_and_hidden_entries_in_a_directory_operand_with_read_all", should_print_visible_and_hidden_entries_in_a_directory_operand_with_read_all);
        CU_add_test(suite, "should_print_visible_entries_in_a_directory_operand_without_hidden_entries_with_read_all", should_print_visible_entries_in_a_directory_operand_without_hidden_entries_with_read_all);
        CU_add_test(suite, "should_print_only_dot_entries_for_an_empty_directory_operand_with_read_all", should_print_only_dot_entries_for_an_empty_directory_operand_with_read_all);
        CU_add_test(suite, "should_print_hidden_entries_in_a_directory_operand_with_only_hidden_entries_with_read_all", should_print_hidden_entries_in_a_directory_operand_with_only_hidden_entries_with_read_all);
        CU_add_test(suite, "should_print_hidden_and_visible_entries_from_multiple_directory_operands_with_read_all", should_print_hidden_and_visible_entries_from_multiple_directory_operands_with_read_all);
        CU_add_test(suite, "should_print_headers_for_empty_and_hidden_only_directory_operands_with_read_all", should_print_headers_for_empty_and_hidden_only_directory_operands_with_read_all);
        CU_add_test(suite, "should_print_hidden_and_visible_entries_from_mixed_operands_with_read_all", should_print_hidden_and_visible_entries_from_mixed_operands_with_read_all);
        CU_add_test(suite, "should_print_mixed_operands_with_empty_and_hidden_only_directories_with_read_all", should_print_mixed_operands_with_empty_and_hidden_only_directories_with_read_all);
        CU_add_test(suite, "should_print_headers_for_all_mixed_operand_directories_with_read_all", should_print_headers_for_all_mixed_operand_directories_with_read_all);
        CU_add_test(suite, "should_print_explicitly_specified_hidden_file_and_directory_operands_with_read_all", should_print_explicitly_specified_hidden_file_and_directory_operands_with_read_all);
        CU_add_test(suite, "should_reverse_sort_the_current_directory_when_reverse_option_is_active", should_reverse_sort_the_current_directory_when_reverse_option_is_active);
        CU_add_test(suite, "should_reverse_sort_a_directory_operand_contents_when_reverse_option_is_active", should_reverse_sort_a_directory_operand_contents_when_reverse_option_is_active);
        CU_add_test(suite, "should_reverse_sort_multiple_file_operands_when_reverse_option_is_active", should_reverse_sort_multiple_file_operands_when_reverse_option_is_active);
        CU_add_test(suite, "should_reverse_sort_multiple_directory_operands_and_their_contents_when_reverse_option_is_active", should_reverse_sort_multiple_directory_operands_and_their_contents_when_reverse_option_is_active);
        CU_add_test(suite, "should_reverse_sort_file_and_directory_operands_separately_and_directory_contents_when_reverse_option_is_active", should_reverse_sort_file_and_directory_operands_separately_and_directory_contents_when_reverse_option_is_active);
        CU_add_test(suite, "should_sort_the_current_directory_by_modification_time_when_no_file_operands_are_specified", should_sort_the_current_directory_by_modification_time_when_no_file_operands_are_specified);
        CU_add_test(suite, "should_sort_a_directory_operand_contents_by_modification_time", should_sort_a_directory_operand_contents_by_modification_time);
        CU_add_test(suite, "should_sort_multiple_file_operands_by_modification_time", should_sort_multiple_file_operands_by_modification_time);
        CU_add_test(suite, "should_sort_multiple_directory_operands_and_their_contents_by_modification_time", should_sort_multiple_directory_operands_and_their_contents_by_modification_time);
        CU_add_test(suite, "should_sort_mixed_file_and_directory_operands_separately_by_modification_time", should_sort_mixed_file_and_directory_operands_separately_by_modification_time);
        CU_add_test(suite, "should_fail_with_a_major_error_and_not_print_anything_if_fails_to_open_the_directory", should_fail_with_a_major_error_and_not_print_anything_if_fails_to_open_the_directory);
        CU_add_test(suite, "should_fail_with_a_major_error_and_not_print_anything_if_fails_to_open_the_current_directory_without_arguments", should_fail_with_a_major_error_and_not_print_anything_if_fails_to_open_the_current_directory_without_arguments);
        CU_add_test(suite, "should_fail_with_a_major_error_and_not_print_anything_if_fails_to_read_the_first_entry_of_the_directory", should_fail_with_a_major_error_and_not_print_anything_if_fails_to_read_the_first_entry_of_the_directory);
        CU_add_test(suite, "should_fail_with_a_major_error_and_only_print_the_non_failed_entries_if_fails_to_read_a_middle_entry_of_the_directory", should_fail_with_a_major_error_and_only_print_the_non_failed_entries_if_fails_to_read_a_middle_entry_of_the_directory);
        CU_add_test(suite, "should_fail_with_a_major_error_print_the_contents_of_the_directory_if_fails_to_close_it", should_fail_with_a_major_error_print_the_contents_of_the_directory_if_fails_to_close_it);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_only_the_non_directory_operand_if_fails_to_scan_the_directory_operand", should_fail_with_a_major_error_and_print_only_the_non_directory_operand_if_fails_to_scan_the_directory_operand);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_only_the_working_directory_operand_if_fails_to_scan_a_directory_operand", should_fail_with_a_major_error_and_print_only_the_working_directory_operand_if_fails_to_scan_a_directory_operand);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_only_the_working_file_operands_if_fails_to_scan_a_directory_operand", should_fail_with_a_major_error_and_print_only_the_working_file_operands_if_fails_to_scan_a_directory_operand);
        CU_add_test(suite, "should_fail_with_a_major_error_and_not_print_anything_if_all_directories_fail_to_scan", should_fail_with_a_major_error_and_not_print_anything_if_all_directories_fail_to_scan);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_only_the_working_file_operands_if_some_directories_fail_to_scan", should_fail_with_a_major_error_and_print_only_the_working_file_operands_if_some_directories_fail_to_scan);
        CU_add_test(suite, "should_fail_with_a_major_error_and_not_print_anything_if_fails_to_access_the_current_directory", should_fail_with_a_major_error_and_not_print_anything_if_fails_to_access_the_current_directory);
        CU_add_test(suite, "should_fail_with_a_major_error_and_not_print_anything_if_the_specified_file_operand_fails_to_be_accessed", should_fail_with_a_major_error_and_not_print_anything_if_the_specified_file_operand_fails_to_be_accessed);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_the_working_operand_if_one_file_operand_fails_to_be_accessed", should_fail_with_a_major_error_and_print_the_working_operand_if_one_file_operand_fails_to_be_accessed);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_only_the_working_operands_if_one_file_operand_fails_to_be_accessed", should_fail_with_a_major_error_and_print_only_the_working_operands_if_one_file_operand_fails_to_be_accessed);
        CU_add_test(suite, "should_fail_with_a_major_error_and_not_print_anything_if_the_specified_file_operands_fail_to_be_accessed", should_fail_with_a_major_error_and_not_print_anything_if_the_specified_file_operands_fail_to_be_accessed);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_only_the_working_operands_if_some_file_operands_fail_to_be_accessed", should_fail_with_a_major_error_and_print_only_the_working_operands_if_some_file_operands_fail_to_be_accessed);
        CU_add_test(suite, "should_fail_with_a_major_error_and_not_print_anything_if_fails_to_access_an_operand_and_process_a_directory", should_fail_with_a_major_error_and_not_print_anything_if_fails_to_access_an_operand_and_process_a_directory);
        CU_add_test(suite, "should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_access_a_child_file_of_a_directory_operand", should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_access_a_child_file_of_a_directory_operand);
        CU_add_test(suite, "should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_open_a_subdirectory", should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_open_a_subdirectory);
        CU_add_test(suite, "should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_read_a_subdirectory", should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_read_a_subdirectory);
        CU_add_test(suite, "should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_close_a_subdirectory", should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_close_a_subdirectory);
        CU_add_test(suite, "should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_access_content_of_a_directory", should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_access_content_of_a_directory);
        CU_add_test(suite, "should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_access_content_of_different_directories", should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_access_content_of_different_directories);
        CU_add_test(suite, "should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_access_and_open_content_of_different_directories", should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_access_and_open_content_of_different_directories);
        CU_add_test(suite, "should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_access_and_read_content_of_a_directory", should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_access_and_read_content_of_a_directory);
        CU_add_test(suite, "should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_access_and_close_content_of_a_directory", should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_access_and_close_content_of_a_directory);
        CU_add_test(suite, "should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_operate_multiple_subdirectories", should_fail_with_a_minor_error_and_print_the_non_failing_files_if_fails_to_operate_multiple_subdirectories);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_access_subdir_content_and_to_open_directory_operand", should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_access_subdir_content_and_to_open_directory_operand);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_read_subdir_content_and_directory_operand", should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_read_subdir_content_and_directory_operand);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_dir_operand_content_and_read_directory_operand", should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_dir_operand_content_and_read_directory_operand);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_open_subdir_content_and_close_operand", should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_open_subdir_content_and_close_operand);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_access_an_operand_and_open_subdirectory", should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_access_an_operand_and_open_subdirectory);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_open_a_directory_operand_and_to_read_subdirectory_content", should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_open_a_directory_operand_and_to_read_subdirectory_content);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_read_a_directory_operand_and_to_close_subdirectory", should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_read_a_directory_operand_and_to_close_subdirectory);
        CU_add_test(suite, "should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_close_a_directory_operand_and_to_access_subdirectory_contents", should_fail_with_a_major_error_and_print_the_non_failing_files_if_fails_to_close_a_directory_operand_and_to_access_subdirectory_contents);
    }
}
