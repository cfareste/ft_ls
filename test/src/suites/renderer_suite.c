#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "renderer.h"
#include "mocks.h"

#define SUITE_NAME "renderer"

static void test_setup(void)
{
    reset_printing_buffer();
}

static void should_create_a_context(void)
{
    t_render_context *context = render_context_create();

    CU_ASSERT_PTR_NOT_NULL(context);

    render_context_destroy(&context);
}

static void should_destroy_a_context(void)
{
    t_render_context *context = render_context_create();

    render_context_destroy(&context);

    CU_ASSERT_PTR_NULL(context);
}

static void should_not_fail_to_destroy_a_context_when_a_null_pointer_is_passed(void)
{
    render_context_destroy(NULL);
}

static void should_not_fail_to_destroy_a_context_when_an_already_null_context_is_passed(void)
{
    t_render_context *invalid = NULL;

    render_context_destroy(&invalid);
}

static void should_not_fail_set_a_directory_header_when_passed_a_null_context(void)
{
    render_context_set_directory_header(NULL, "valid");
}

static void should_not_fail_set_a_null_directory_header(void)
{
    t_render_context *valid = render_context_create();

    render_context_set_directory_header(valid, NULL);

    render_context_destroy(&valid);
}

static void should_not_print_anything_if_the_file_entry_array_is_null(void)
{
    t_render_context *context = render_context_create();

    render(NULL, context);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(""));

    render_context_destroy(&context);
}

static void should_not_print_anything_if_the_context_is_null(void)
{
    t_file_entry_array *file_entry_array = file_entry_array_create();
    t_file_entry *file_entry = file_entry_create("valid");
    file_entry_array_push(file_entry_array, file_entry);

    render(file_entry_array, NULL);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(""));

    file_entry_array_destroy(&file_entry_array);
}

static void should_print_the_name_of_the_entry_with_a_file_entry_array_of_one_element(void)
{
    const char *expected_file_name = "file";
    t_file_entry_array *file_entry_array = file_entry_array_create();
    t_file_entry *file_entry = file_entry_create(expected_file_name);
    file_entry_array_push(file_entry_array, file_entry);
    t_render_context *context = render_context_create();

    render(file_entry_array, context);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is("%s\n", expected_file_name));

    file_entry_array_destroy(&file_entry_array);
    render_context_destroy(&context);
}

static void should_print_the_name_of_every_entry_with_a_file_entry_array_of_various_elements_separated_by_new_lines(void)
{
    t_render_context *context = render_context_create();
    const char *expected_file_name[] = { "file", "file2", "file3", "file4", "file5" };
    t_file_entry_array *file_entry_array = file_entry_array_create();
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[0]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[1]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[2]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[3]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[4]));

    render(file_entry_array, context);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(
        "%s\n%s\n%s\n%s\n%s\n",
        expected_file_name[0],
        expected_file_name[1],
        expected_file_name[2],
        expected_file_name[3],
        expected_file_name[4]
    ));

    file_entry_array_destroy(&file_entry_array);
    render_context_destroy(&context);
}

static void should_not_print_a_dir_header_if_specified_in_the_context(void)
{
    const char *expected_file_name[] = { "file", "file2", "file3" };
    t_render_context *context = render_context_create();
    render_context_set_directory_header(context, NULL);
    t_file_entry_array *file_entry_array = file_entry_array_create();
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[0]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[1]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[2]));

    render(file_entry_array, context);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(
        "%s\n%s\n%s\n",
        expected_file_name[0],
        expected_file_name[1],
        expected_file_name[2]
    ));

    file_entry_array_destroy(&file_entry_array);
    render_context_destroy(&context);
}

static void should_not_print_a_leading_dir_header_newline_if_its_the_first_render(void)
{
    const char *expected_file_name[] = { "file", "file2", "file3" };
    const char *dir_header = "dir";
    t_render_context *context = render_context_create();
    render_context_set_directory_header(context, dir_header);
    t_file_entry_array *file_entry_array = file_entry_array_create();
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[0]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[1]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[2]));

    render(file_entry_array, context);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(
        "%s:\n%s\n%s\n%s\n",
        dir_header,
        expected_file_name[0],
        expected_file_name[1],
        expected_file_name[2]
    ));

    file_entry_array_destroy(&file_entry_array);
    render_context_destroy(&context);
}

static void should_print_a_leading_dir_header_newline_if_its_not_first_render(void)
{
    const char *expected_file_name[] = { "file", "file2" };
    const char *dir_header = "dir";
    t_render_context *context = render_context_create();
    render_context_set_directory_header(context, dir_header);
    t_file_entry_array *file_entry_array = file_entry_array_create();
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[0]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[1]));

    render(file_entry_array, context);
    render(file_entry_array, context);

    CU_ASSERT(verify_that_the_str_that_has_been_printed_is(
        "%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n",
        dir_header,
        expected_file_name[0],
        expected_file_name[1],
        dir_header,
        expected_file_name[0],
        expected_file_name[1]
    ));

    file_entry_array_destroy(&file_entry_array);
    render_context_destroy(&context);
}

/*static void should_print_the_name_of_every_entry_with_a_file_entry_array_of_various_elements_separated_by_two_spaces(void)
{
    const char *expected_file_name[] = { "file", "file2", "file3", "file4", "file5" };
    t_file_entry_array *file_entry_array = file_entry_array_create(expected_file_name[0]);
    file_entry_array_push(&file_entry_array, file_entry_array_create(expected_file_name[1]));
    file_entry_array_push(&file_entry_array, file_entry_array_create(expected_file_name[2]));
    file_entry_array_push(&file_entry_array, file_entry_array_create(expected_file_name[3]));
    file_entry_array_push(&file_entry_array, file_entry_array_create(expected_file_name[4]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[0]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[1]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[2]));

    render(file_entry_array);

    verify_that_the_str_that_has_been_printed_is(
        "%s  %s  %s  %s  %s\n",
        expected_file_name[0],
        expected_file_name[1],
        expected_file_name[2],
        expected_file_name[3],
        expected_file_name[4]
    );

    file_entry_array_destroy(&file_entry_array);
}*/

void register_renderer_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_create_a_context", should_create_a_context);
        CU_add_test(suite, "should_destroy_a_context", should_destroy_a_context);
        CU_add_test(suite, "should_not_fail_to_destroy_a_context_when_a_null_pointer_is_passed", should_not_fail_to_destroy_a_context_when_a_null_pointer_is_passed);
        CU_add_test(suite, "should_not_fail_to_destroy_a_context_when_an_already_null_context_is_passed", should_not_fail_to_destroy_a_context_when_an_already_null_context_is_passed);
        CU_add_test(suite, "should_not_fail_set_a_directory_header_when_passed_a_null_context", should_not_fail_set_a_directory_header_when_passed_a_null_context);
        CU_add_test(suite, "should_not_fail_set_a_null_directory_header", should_not_fail_set_a_null_directory_header);
        CU_add_test(suite, "should_not_print_anything_if_the_file_entry_array_is_null", should_not_print_anything_if_the_file_entry_array_is_null);
        CU_add_test(suite, "should_not_print_anything_if_the_context_is_null", should_not_print_anything_if_the_context_is_null);
        CU_add_test(suite, "should_print_the_name_of_the_entry_with_a_file_entry_array_of_one_element", should_print_the_name_of_the_entry_with_a_file_entry_array_of_one_element);
        CU_add_test(suite, "should_print_the_name_of_every_entry_with_a_file_entry_array_of_various_elements_separated_by_new_lines", should_print_the_name_of_every_entry_with_a_file_entry_array_of_various_elements_separated_by_new_lines);
        CU_add_test(suite, "should_not_print_a_dir_header_if_specified_in_the_context", should_not_print_a_dir_header_if_specified_in_the_context);
        CU_add_test(suite, "should_not_print_a_leading_dir_header_newline_if_its_the_first_render", should_not_print_a_leading_dir_header_newline_if_its_the_first_render);
        CU_add_test(suite, "should_print_a_leading_dir_header_newline_if_its_not_first_render", should_print_a_leading_dir_header_newline_if_its_not_first_render);
        // CU_add_test(suite, "should_print_the_name_of_every_entry_with_a_file_entry_array_of_various_elements_separated_by_two_spaces", should_print_the_name_of_every_entry_with_a_file_entry_array_of_various_elements_separated_by_two_spaces);
    }
}
