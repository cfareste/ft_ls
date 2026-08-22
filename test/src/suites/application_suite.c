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
    const char *arguments[] = { NULL };
    const char *dir_names[] = { ".", NULL };
    const char *file_names[7] = { ".", "..", "file1", "subdir1", "symlink", "zz", NULL };
    const char **entry_names[] = { file_names, NULL };
    guarantee_stat_will_populate_stats_of_a_directory_type_file(".");
    ensure_opendir_will_open_N_dirs_named(dir_names);
    guarantee_readdir_will_return_N_files_named(entry_names);
    parsed_arguments = parse_arguments(0, arguments);

    const int result = run_application(parsed_arguments);

    verify_that_the_str_that_has_been_printed_is("%s\n%s\n%s\n%s\n%s\n%s\n",
        file_names[0],
        file_names[1],
        file_names[2],
        file_names[3],
        file_names[4],
        file_names[5]
    );
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_file_name_if_a_regular_file_operand_is_specified(void)
{
    const char *file_name = "../../ft_ls/test/./frameworks/../regular_file";
    const char *arguments[] = { file_name, NULL };
    guarantee_stat_will_populate_stats_of_a_regular_type_file(file_name);
    parsed_arguments = parse_arguments(1, arguments);

    const int result = run_application(parsed_arguments);

    verify_that_the_str_that_has_been_printed_is("%s\n", file_name);
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_contents_of_the_directory_specified_as_an_operand(void)
{
    const char *arguments[] = { "dir", NULL };
    const char *dir_names[] = { arguments[0], NULL };
    const char *file_names[7] = { ".", "..", "file_from_dir_1", "subdir_1", "block_device", "char_device", NULL };
    const char **entry_names[] = { file_names, NULL };
    guarantee_stat_will_populate_stats_of_a_directory_type_file(arguments[0]);
    ensure_opendir_will_open_N_dirs_named(dir_names);
    guarantee_readdir_will_return_N_files_named(entry_names);
    parsed_arguments = parse_arguments(1, arguments);

    const int result = run_application(parsed_arguments);

    verify_that_the_str_that_has_been_printed_is("%s\n%s\n%s\n%s\n%s\n%s\n",
        file_names[0],
        file_names[1],
        file_names[2],
        file_names[3],
        file_names[4],
        file_names[5]
    );
    assert_application_execution_succeed(result);
}

static void should_successfully_print_the_contents_of_the_mixed_types_specified_operands(void)
{
    const char *arguments[] = { "dir", "file1", "file1", "symlink", "dir1", NULL };
    const unsigned int types[] = { S_IFDIR, S_IFREG, S_IFREG, S_IFLNK, S_IFDIR, 0 };
    const char *dir_names[] = { arguments[0], arguments[4], NULL };
    const char *first_dir_file_names[] = { ".", "file_from_dir_1", "subdir_1", "block_device", "..", "char_device", NULL };
    const char *second_dir_file_names[] = { "..", "file_from_dir_2", ".", "symlink", "file2", NULL };
    const char **entry_names[] = { first_dir_file_names, second_dir_file_names, NULL };
    guarantee_stat_will_populate_stats_of_N_file_types_for_paths(arguments, types);
    ensure_opendir_will_open_N_dirs_named(dir_names);
    guarantee_readdir_will_return_N_files_named(entry_names);
    parsed_arguments = parse_arguments(5, arguments);

    const int result = run_application(parsed_arguments);

    verify_that_the_str_that_has_been_printed_is(
        "%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n%s\n%s\n%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n%s\n%s\n%s\n",
        arguments[1],
        arguments[2],
        arguments[3],
        dir_names[0],
        first_dir_file_names[0],
        first_dir_file_names[1],
        first_dir_file_names[2],
        first_dir_file_names[3],
        first_dir_file_names[4],
        first_dir_file_names[5],
        dir_names[1],
        second_dir_file_names[0],
        second_dir_file_names[1],
        second_dir_file_names[2],
        second_dir_file_names[3],
        second_dir_file_names[4]
    );
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
        CU_add_test(suite, "should_successfully_print_the_contents_of_the_mixed_types_specified_operands", should_successfully_print_the_contents_of_the_mixed_types_specified_operands);
    }
}
