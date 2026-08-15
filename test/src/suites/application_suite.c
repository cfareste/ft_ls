#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "mocks.h"
#include "application.h"

#define SUITE_NAME "application"

static void test_setup(void)
{
    reset_dirent_guarantees();
    reset_stat_guarantees();
    reset_printing_buffer();
}

static void should_successfully_print_the_contents_of_the_current_directory_one_per_line_if_no_file_operands_are_specified(void)
{
    const char *arguments[] = { NULL };
    const char *file_names[7] = { ".", "..", "file1", "subdir1", "symlink", "zz", NULL };
    t_ft_ls_options *options = ft_ls_options_get(0, arguments);
    guarantee_stat_will_populate_stats_of_a_directory_type_file();
    ensure_opendir_will_open_a_dir_named(".");
    guarantee_readdir_will_return_N_files_named(file_names);

    const int result = run_application(options);

    verify_that_the_str_that_has_been_printed_is("%s\n%s\n%s\n%s\n%s\n%s\n",
        file_names[0],
        file_names[1],
        file_names[2],
        file_names[3],
        file_names[4],
        file_names[5],
        file_names[6]
    );
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_SUCCESS);

    ft_ls_options_destroy(&options);
}

static void should_successfully_print_the_file_name_if_a_regular_file_operand_is_specified(void)
{
    const char *file_name = "regular file";
    const char *arguments[] = { file_name, NULL };
    t_ft_ls_options *options = ft_ls_options_get(1, arguments);
    guarantee_stat_will_populate_stats_of_a_regular_type_file();
    guarantee_readdir_will_return_a_file_named(file_name);

    const int result = run_application(options);

    verify_that_the_str_that_has_been_printed_is("%s\n", file_name);
    CU_ASSERT_EQUAL(result, FT_LS_APPLICATION_SUCCESS);

    ft_ls_options_destroy(&options);
}

void register_application_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_successfully_print_the_contents_of_the_current_directory_one_per_line_if_no_file_operands_are_specified", should_successfully_print_the_contents_of_the_current_directory_one_per_line_if_no_file_operands_are_specified);
        CU_add_test(suite, "should_successfully_print_the_file_name_if_a_regular_file_operand_is_specified", should_successfully_print_the_file_name_if_a_regular_file_operand_is_specified);
    }
}
