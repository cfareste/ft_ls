#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "renderer.h"
#include "mocks.h"

#define SUITE_NAME "renderer"

static t_parsed_arguments *parsed_arguments;

static void test_setup(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);
    const char *args[] = { NULL };
    parsed_arguments = parse_arguments(0, args);
    reset_printing_buffer();
}

static void test_teardown(void)
{
    parsed_arguments_destroy(&parsed_arguments);
    vfs_mock_reset();
}

static void should_create_a_context(void)
{
    t_render_context *context = render_context_create(parsed_arguments);

    CU_ASSERT_PTR_NOT_NULL(context);

    render_context_destroy(&context);
}

static void should_return_NULL_when_creating_a_context_if_null_parsed_args_are_specified(void)
{
    t_render_context *context = render_context_create(NULL);

    CU_ASSERT_PTR_NULL(context);

    render_context_destroy(&context);
}

static void should_destroy_a_context(void)
{
    t_render_context *context = render_context_create(parsed_arguments);

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

static void should_not_print_anything_if_the_file_entry_array_is_null(void)
{
    t_render_context *context = render_context_create(parsed_arguments);

    render(NULL, context);

    CU_ASSERT(verify_that_no_output_was_printed());

    render_context_destroy(&context);
}

static void should_not_print_anything_if_the_context_is_null(void)
{
    t_file_entry_array *file_entry_array = file_entry_array_create();
    t_file_entry *file_entry = file_entry_create("valid");
    file_entry_array_push(file_entry_array, file_entry);

    render(file_entry_array, NULL);

    CU_ASSERT(verify_that_no_output_was_printed());

    file_entry_array_destroy(&file_entry_array);
}

static void should_print_the_name_of_the_entry_with_a_file_entry_array_of_one_element(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *expected_file_name = "file";
    const char *args[] = { expected_file_name, NULL };
    t_parsed_arguments *parsed_args = parse_arguments(1, args);
    t_file_entry_array *file_entry_array = file_entry_array_create();
    t_file_entry *file_entry = file_entry_create(expected_file_name);
    file_entry_array_push(file_entry_array, file_entry);
    t_render_context *context = render_context_create(parsed_args);

    render(file_entry_array, context);

    CU_ASSERT(verify_that_the_output_printed_is("%s\n", expected_file_name));

    file_entry_array_destroy(&file_entry_array);
    parsed_arguments_destroy(&parsed_args);
    render_context_destroy(&context);
}

static void should_print_the_name_of_every_entry_with_a_file_entry_array_of_various_elements_separated_by_new_lines(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_FILE("file3"),
        MOCK_FILE("file4"),
        MOCK_FILE("file5"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "file", "file2", "file3", "file4", "file5", NULL };
    t_parsed_arguments *parsed_args = parse_arguments(5, args);
    t_render_context *context = render_context_create(parsed_args);
    const char *expected_file_name[] = { "file", "file2", "file3", "file4", "file5" };
    t_file_entry_array *file_entry_array = file_entry_array_create();
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[0]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[1]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[2]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[3]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[4]));

    render(file_entry_array, context);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n%s\n%s\n%s\n",
        expected_file_name[0],
        expected_file_name[1],
        expected_file_name[2],
        expected_file_name[3],
        expected_file_name[4]
    ));

    file_entry_array_destroy(&file_entry_array);
    parsed_arguments_destroy(&parsed_args);
    render_context_destroy(&context);
}

static void should_not_print_a_leading_dir_header_newline_if_its_the_first_render(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", ".."),
        MOCK_DIR("dir2", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *expected_file_name[] = { "file", "file2", "file3" };
    const char *dir_header = "dir";
    const char *args[] = { "dir", "dir2", NULL };
    t_parsed_arguments *parsed_args = parse_arguments(2, args);
    t_render_context *context = render_context_create(parsed_args);
    t_file_entry_array *file_entry_array = file_entry_array_create();
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[0]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[1]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[2]));

    render_directory(context, dir_header, file_entry_array);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s:\n%s\n%s\n%s\n",
        dir_header,
        expected_file_name[0],
        expected_file_name[1],
        expected_file_name[2]
    ));

    file_entry_array_destroy(&file_entry_array);
    parsed_arguments_destroy(&parsed_args);
    render_context_destroy(&context);
}

static void should_print_a_leading_dir_header_newline_if_its_not_first_render(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", ".."),
        MOCK_DIR("dir2", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *expected_file_name[] = { "file", "file2" };
    const char *dir_header = "dir";
    const char *args[] = { "dir", "dir2", NULL };
    t_parsed_arguments *parsed_args = parse_arguments(2, args);
    t_render_context *context = render_context_create(parsed_args);
    t_file_entry_array *file_entry_array = file_entry_array_create();
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[0]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[1]));

    render_directory(context, dir_header, file_entry_array);
    render_directory(context, dir_header, file_entry_array);

    CU_ASSERT(verify_that_the_output_printed_is(
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
    parsed_arguments_destroy(&parsed_args);
    render_context_destroy(&context);
}

static void should_not_print_the_types_separator_if_NULL_context_is_passed(void)
{
    render_types_separator(NULL);

    CU_ASSERT(verify_that_no_output_was_printed());
}

static void should_print_the_types_separator_if_has_at_least_one_non_dir_and_one_dir(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", ".."),
        MOCK_FILE("file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "dir", "file", NULL };
    t_parsed_arguments *parsed_args = parse_arguments(2, args);
    t_render_context *context = render_context_create(parsed_args);

    render_types_separator(context);

    CU_ASSERT(verify_that_the_output_printed_is("\n"));

    parsed_arguments_destroy(&parsed_args);
    render_context_destroy(&context);
}

static void should_not_print_the_types_separator_if_doesnt_have_at_least_one_non_dir_and_one_dir(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "file2", "file", NULL };
    t_parsed_arguments *parsed_args = parse_arguments(2, args);
    t_render_context *context = render_context_create(parsed_args);

    render_types_separator(context);

    CU_ASSERT(verify_that_no_output_was_printed());

    parsed_arguments_destroy(&parsed_args);
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

    verify_that_the_output_printed_is(
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
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, test_teardown);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_create_a_context", should_create_a_context);
        CU_add_test(suite, "should_return_NULL_when_creating_a_context_if_null_parsed_args_are_specified", should_return_NULL_when_creating_a_context_if_null_parsed_args_are_specified);
        CU_add_test(suite, "should_destroy_a_context", should_destroy_a_context);
        CU_add_test(suite, "should_not_fail_to_destroy_a_context_when_a_null_pointer_is_passed", should_not_fail_to_destroy_a_context_when_a_null_pointer_is_passed);
        CU_add_test(suite, "should_not_fail_to_destroy_a_context_when_an_already_null_context_is_passed", should_not_fail_to_destroy_a_context_when_an_already_null_context_is_passed);
        CU_add_test(suite, "should_not_print_anything_if_the_file_entry_array_is_null", should_not_print_anything_if_the_file_entry_array_is_null);
        CU_add_test(suite, "should_not_print_anything_if_the_context_is_null", should_not_print_anything_if_the_context_is_null);
        CU_add_test(suite, "should_print_the_name_of_the_entry_with_a_file_entry_array_of_one_element", should_print_the_name_of_the_entry_with_a_file_entry_array_of_one_element);
        CU_add_test(suite, "should_print_the_name_of_every_entry_with_a_file_entry_array_of_various_elements_separated_by_new_lines", should_print_the_name_of_every_entry_with_a_file_entry_array_of_various_elements_separated_by_new_lines);
        CU_add_test(suite, "should_not_print_a_leading_dir_header_newline_if_its_the_first_render", should_not_print_a_leading_dir_header_newline_if_its_the_first_render);
        CU_add_test(suite, "should_print_a_leading_dir_header_newline_if_its_not_first_render", should_print_a_leading_dir_header_newline_if_its_not_first_render);
        CU_add_test(suite, "should_not_print_the_types_separator_if_NULL_context_is_passed", should_not_print_the_types_separator_if_NULL_context_is_passed);
        CU_add_test(suite, "should_print_the_types_separator_if_has_at_least_one_non_dir_and_one_dir", should_print_the_types_separator_if_has_at_least_one_non_dir_and_one_dir);
        CU_add_test(suite, "should_not_print_the_types_separator_if_doesnt_have_at_least_one_non_dir_and_one_dir", should_not_print_the_types_separator_if_doesnt_have_at_least_one_non_dir_and_one_dir);
        // CU_add_test(suite, "should_print_the_name_of_every_entry_with_a_file_entry_array_of_various_elements_separated_by_two_spaces", should_print_the_name_of_every_entry_with_a_file_entry_array_of_various_elements_separated_by_two_spaces);
    }
}
