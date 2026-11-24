#include "suites.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "options.h"

#define SUITE_NAME "options"

static void should_return_no_options_for_a_NULL_argument_list(void)
{
    CU_ASSERT_EQUAL(options_get(NULL), OPTIONS_NONE);
}

static void should_return_no_options_for_an_empty_argument_list(void)
{
    const char *arguments[] = { NULL };

    CU_ASSERT_EQUAL(options_get(arguments), OPTIONS_NONE);
}

static void should_combine_all_supported_options_including_combined_flags(void)
{
    const char *arguments[] = { "-Rldg", "-Car", "-tU", "--color", "file", NULL };
    const t_options expected = OPTIONS_RECURSIVE | OPTIONS_LONG_FORMAT
        | OPTIONS_MULTI_COLUMN_FORMAT | OPTIONS_READ_ALL | OPTIONS_REVERSE
        | OPTIONS_TIME_SORTING | OPTIONS_COLOR | OPTIONS_NO_SORT | OPTIONS_IMMEDIATE_DIRS
        | OPTIONS_LONG_FORMAT_NO_OWNER;

    CU_ASSERT_EQUAL(options_get(arguments), expected);
}

static void should_accept_d_as_a_standalone_option(void)
{
    const char *arguments[] = { "-d", NULL };

    CU_ASSERT_EQUAL(options_get(arguments), OPTIONS_IMMEDIATE_DIRS);
    CU_ASSERT_EQUAL(options_get_invalid_option(arguments), '\0');
    CU_ASSERT_EQUAL(options_get_amount(arguments), 1);
}

static void should_return_zero_option_count_for_a_NULL_argument_list(void)
{
    CU_ASSERT_EQUAL(options_get_amount(NULL), 0);
}

static void should_accept_g_as_a_standalone_option(void)
{
    const char *arguments[] = { "-g", NULL };

    CU_ASSERT_EQUAL(options_get(arguments), OPTIONS_LONG_FORMAT_NO_OWNER);
    CU_ASSERT_EQUAL(options_get_invalid_option(arguments), '\0');
    CU_ASSERT_EQUAL(options_get_amount(arguments), 1);
}

static void should_expand_f_to_read_all_and_no_sort(void)
{
    const char *arguments[] = { "-f", NULL };

    CU_ASSERT_EQUAL(options_get(arguments), OPTIONS_READ_ALL | OPTIONS_NO_SORT);
    CU_ASSERT_EQUAL(options_get_invalid_option(arguments), '\0');
    CU_ASSERT_EQUAL(options_get_amount(arguments), 1);
}

static void should_preserve_other_options_when_combined_with_f(void)
{
    const char *arguments[] = { "-lf", "--color", NULL };

    CU_ASSERT_EQUAL(options_get(arguments), OPTIONS_READ_ALL | OPTIONS_NO_SORT | OPTIONS_LONG_FORMAT | OPTIONS_COLOR);
    CU_ASSERT_EQUAL(options_get_invalid_option(arguments), '\0');
}

static void should_count_option_arguments_not_individual_flags(void)
{
    const char *arguments[] = { "-Rl", "file", "-art", NULL };

    CU_ASSERT_EQUAL(options_get_amount(arguments), 2);
}

static void should_return_no_invalid_option_for_a_NULL_argument_list(void)
{
    CU_ASSERT_EQUAL(options_get_invalid_option(NULL), '\0');
}

static void should_return_no_invalid_option_for_supported_and_non_option_arguments(void)
{
    const char *arguments[] = { "-RlaCd", "-U", "--color", "file", NULL };

    CU_ASSERT_EQUAL(options_get_invalid_option(arguments), '\0');
}

static void should_return_the_first_unsupported_option_character(void)
{
    const char *arguments[] = { "-laX", "-z", NULL };

    CU_ASSERT_EQUAL(options_get_invalid_option(arguments), 'X');
}

void register_options_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, NULL, NULL);

    if (suite == NULL)
        return ;

    CU_add_test(suite, "should_return_no_options_for_a_NULL_argument_list", should_return_no_options_for_a_NULL_argument_list);
    CU_add_test(suite, "should_return_no_options_for_an_empty_argument_list", should_return_no_options_for_an_empty_argument_list);
    CU_add_test(suite, "should_combine_all_supported_options_including_combined_flags", should_combine_all_supported_options_including_combined_flags);
    CU_add_test(suite, "should_accept_d_as_a_standalone_option", should_accept_d_as_a_standalone_option);
    CU_add_test(suite, "should_accept_g_as_a_standalone_option", should_accept_g_as_a_standalone_option);
    CU_add_test(suite, "should_expand_f_to_read_all_and_no_sort", should_expand_f_to_read_all_and_no_sort);
    CU_add_test(suite, "should_preserve_other_options_when_combined_with_f", should_preserve_other_options_when_combined_with_f);
    CU_add_test(suite, "should_return_zero_option_count_for_a_NULL_argument_list", should_return_zero_option_count_for_a_NULL_argument_list);
    CU_add_test(suite, "should_count_option_arguments_not_individual_flags", should_count_option_arguments_not_individual_flags);
    CU_add_test(suite, "should_return_no_invalid_option_for_a_NULL_argument_list", should_return_no_invalid_option_for_a_NULL_argument_list);
    CU_add_test(suite, "should_return_no_invalid_option_for_supported_and_non_option_arguments", should_return_no_invalid_option_for_supported_and_non_option_arguments);
    CU_add_test(suite, "should_return_the_first_unsupported_option_character", should_return_the_first_unsupported_option_character);
}
