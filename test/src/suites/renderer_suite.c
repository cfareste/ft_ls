#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "renderer.h"
#include "mocks.h"

#define SUITE_NAME "renderer"

static void test_setup()
{
    reset_printing_buffer();
}

static void should_not_print_anything_if_the_file_entry_list_is_null(void)
{
    t_render_config *config = render_config_create_for_non_directory_file_operands();

    render(NULL, config);

    verify_that_the_str_that_has_been_printed_is("");

    render_config_destroy(&config);
}

static void should_print_the_name_of_the_entry_with_a_file_entry_list_of_one_element(void)
{
    const char *expected_file_name = "file";
    t_file_entry_list *file_entry_list = file_entry_list_create(expected_file_name);
    t_render_config *config = render_config_create_for_non_directory_file_operands();

    render(file_entry_list, config);

    verify_that_the_str_that_has_been_printed_is("%s\n", expected_file_name);

    file_entry_list_destroy(&file_entry_list);
    render_config_destroy(&config);
}

static void should_print_the_name_of_every_entry_with_a_file_entry_list_of_various_elements_separated_by_new_lines(void)
{
    t_render_config *config = render_config_create_for_non_directory_file_operands();
    const char *expected_file_name[5] = { "file", "file2", "file3", "file4", "file5" };
    t_file_entry_list *file_entry_list = file_entry_list_create(expected_file_name[0]);
    file_entry_list_add_entry(&file_entry_list, file_entry_list_create(expected_file_name[1]));
    file_entry_list_add_entry(&file_entry_list, file_entry_list_create(expected_file_name[2]));
    file_entry_list_add_entry(&file_entry_list, file_entry_list_create(expected_file_name[3]));
    file_entry_list_add_entry(&file_entry_list, file_entry_list_create(expected_file_name[4]));

    render(file_entry_list, config);

    verify_that_the_str_that_has_been_printed_is(
        "%s\n%s\n%s\n%s\n%s\n",
        expected_file_name[0],
        expected_file_name[1],
        expected_file_name[2],
        expected_file_name[3],
        expected_file_name[4]
    );

    file_entry_list_destroy(&file_entry_list);
    render_config_destroy(&config);
}

static void should_print_a_leading_dir_header_if_a_directory_config_is_specified(void)
{
    const char *expected_file_name[3] = { "file", "file2", "file3" };
    const char *dir_header = "dir";
    t_render_config *config = render_config_create_for_directory_file_operands(dir_header);
    t_file_entry_list *file_entry_list = file_entry_list_create(expected_file_name[0]);
    file_entry_list_add_entry(&file_entry_list, file_entry_list_create(expected_file_name[1]));
    file_entry_list_add_entry(&file_entry_list, file_entry_list_create(expected_file_name[2]));

    render(file_entry_list, config);

    verify_that_the_str_that_has_been_printed_is(
        "\n%s:\n%s\n%s\n%s\n",
        dir_header,
        expected_file_name[0],
        expected_file_name[1],
        expected_file_name[2]
    );

    file_entry_list_destroy(&file_entry_list);
    render_config_destroy(&config);
}

static void should_create_a_configuration_for_non_directory_file_operands(void)
{
    t_render_config *config = render_config_create_for_non_directory_file_operands();

    CU_ASSERT_PTR_NOT_NULL(config);

    render_config_destroy(&config);
}

static void should_create_a_configuration_for_directory_file_operands(void)
{
    t_render_config *config = render_config_create_for_directory_file_operands("dir");

    CU_ASSERT_PTR_NOT_NULL(config);

    render_config_destroy(&config);
}

static void should_destroy_a_configuration(void)
{
    t_render_config *config = render_config_create_for_non_directory_file_operands();

    render_config_destroy(&config);

    CU_ASSERT_PTR_NULL(config);
}

static void should_not_fail_to_destroy_a_configuration_when_a_null_pointer_is_passed(void)
{
    render_config_destroy(NULL);
}

static void should_not_fail_to_destroy_a_configuration_when_an_already_null_configuration_is_passed(void)
{
    t_render_config *invalid = NULL;

    render_config_destroy(&invalid);
}

/*static void should_print_the_name_of_every_entry_with_a_file_entry_list_of_various_elements_separated_by_two_spaces(void)
{
    const char *expected_file_name[5] = { "file", "file2", "file3", "file4", "file5" };
    t_file_entry_list *file_entry_list = file_entry_list_create(expected_file_name[0]);
    file_entry_list_add_entry(&file_entry_list, file_entry_list_create(expected_file_name[1]));
    file_entry_list_add_entry(&file_entry_list, file_entry_list_create(expected_file_name[2]));
    file_entry_list_add_entry(&file_entry_list, file_entry_list_create(expected_file_name[3]));
    file_entry_list_add_entry(&file_entry_list, file_entry_list_create(expected_file_name[4]));

    render(file_entry_list);

    verify_that_the_str_that_has_been_printed_is(
        "%s  %s  %s  %s  %s\n",
        expected_file_name[0],
        expected_file_name[1],
        expected_file_name[2],
        expected_file_name[3],
        expected_file_name[4]
    );

    file_entry_list_destroy(&file_entry_list);
}*/

void register_renderer_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_not_print_anything_if_the_file_entry_list_is_null", should_not_print_anything_if_the_file_entry_list_is_null);
        CU_add_test(suite, "should_print_the_name_of_the_entry_with_a_file_entry_list_of_one_element", should_print_the_name_of_the_entry_with_a_file_entry_list_of_one_element);
        CU_add_test(suite, "should_print_the_name_of_every_entry_with_a_file_entry_list_of_various_elements_separated_by_new_lines", should_print_the_name_of_every_entry_with_a_file_entry_list_of_various_elements_separated_by_new_lines);
        CU_add_test(suite, "should_print_a_leading_dir_header_if_a_directory_config_is_specified", should_print_a_leading_dir_header_if_a_directory_config_is_specified);
        CU_add_test(suite, "should_create_a_configuration_for_non_directory_file_operands", should_create_a_configuration_for_non_directory_file_operands);
        CU_add_test(suite, "should_create_a_configuration_for_directory_file_operands", should_create_a_configuration_for_directory_file_operands);
        CU_add_test(suite, "should_destroy_a_configuration", should_destroy_a_configuration);
        CU_add_test(suite, "should_not_fail_to_destroy_a_configuration_when_a_null_pointer_is_passed", should_not_fail_to_destroy_a_configuration_when_a_null_pointer_is_passed);
        CU_add_test(suite, "should_not_fail_to_destroy_a_configuration_when_an_already_null_configuration_is_passed", should_not_fail_to_destroy_a_configuration_when_an_already_null_configuration_is_passed);
        // CU_add_test(suite, "should_print_the_name_of_every_entry_with_a_file_entry_list_of_various_elements_separated_by_two_spaces", should_print_the_name_of_every_entry_with_a_file_entry_list_of_various_elements_separated_by_two_spaces);
    }
}
