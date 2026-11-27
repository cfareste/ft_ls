#include "suites.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "options.h"

#define SUITE_NAME "options"

static void should_resolve_defaults_without_options(void)
{
    const char *arguments[] = { "file", NULL };
    const t_parsed_options result = options_parse(arguments, 0);

    CU_ASSERT_EQUAL(result.invalid_option, '\0');
    CU_ASSERT_EQUAL(result.options_amount, 0);
    CU_ASSERT_EQUAL(result.options.format, FORMAT_SINGLE_COLUMN);
    CU_ASSERT_EQUAL(result.options.sort_type, SORT_NAME);
    CU_ASSERT_EQUAL(result.options.time_type, TIME_MTIME);
    CU_ASSERT_TRUE(result.options.print_owner);
    CU_ASSERT_TRUE(result.options.filter_hidden_files);
    CU_ASSERT_TRUE(result.options.dereference_symlinks);
    CU_ASSERT_FALSE(result.options.sort_reverse);
    CU_ASSERT_FALSE(result.options.immediate_dirs);
    CU_ASSERT_FALSE(result.options.recursion);
    CU_ASSERT_FALSE(result.options.color_files);
}

static void should_default_to_columns_on_a_terminal(void)
{
    const t_parsed_options result = options_parse(NULL, 1);

    CU_ASSERT_EQUAL(result.invalid_option, '\0');
    CU_ASSERT_EQUAL(result.options_amount, 0);
    CU_ASSERT_EQUAL(result.options.format, FORMAT_COLUMNS);
    CU_ASSERT_EQUAL(result.options.sort_type, SORT_NAME);
}

static void should_accumulate_independent_settings_and_count_option_arguments(void)
{
    const char *arguments[] = { "-Raltr", "file", "--color", NULL };
    const t_parsed_options result = options_parse(arguments, 1);

    CU_ASSERT_EQUAL(result.invalid_option, '\0');
    CU_ASSERT_EQUAL(result.options_amount, 2);
    CU_ASSERT_EQUAL(result.options.format, FORMAT_LONG);
    CU_ASSERT_EQUAL(result.options.sort_type, SORT_TIME);
    CU_ASSERT_EQUAL(result.options.time_type, TIME_MTIME);
    CU_ASSERT_TRUE(result.options.print_owner);
    CU_ASSERT_FALSE(result.options.filter_hidden_files);
    CU_ASSERT_FALSE(result.options.dereference_symlinks);
    CU_ASSERT_TRUE(result.options.sort_reverse);
    CU_ASSERT_TRUE(result.options.recursion);
    CU_ASSERT_TRUE(result.options.color_files);
}

static void should_use_the_last_explicit_format_and_preserve_owner_preference(void)
{
    const char *columns[] = { "-lg", "-C", NULL };
    const char *long_without_owner[] = { "-Cgl", NULL };
    const t_parsed_options column_result = options_parse(columns, 0);
    const t_parsed_options long_result = options_parse(long_without_owner, 0);

    CU_ASSERT_EQUAL(column_result.invalid_option, '\0');
    CU_ASSERT_EQUAL(column_result.options.format, FORMAT_COLUMNS);
    CU_ASSERT_FALSE(column_result.options.print_owner);
    CU_ASSERT_TRUE(column_result.options.dereference_symlinks);
    CU_ASSERT_EQUAL(long_result.invalid_option, '\0');
    CU_ASSERT_EQUAL(long_result.options.format, FORMAT_LONG);
    CU_ASSERT_FALSE(long_result.options.print_owner);
    CU_ASSERT_FALSE(long_result.options.dereference_symlinks);
}

static void should_resolve_explicit_and_implicit_time_sorting(void)
{
    const struct {
        const char *flags;
        t_sort_type sort_type;
        t_time_type time_type;
    } cases[] = {
        { "-u", SORT_TIME, TIME_ATIME },
        { "-ut", SORT_TIME, TIME_ATIME },
        { "-lu", SORT_NAME, TIME_ATIME },
        { "-gu", SORT_NAME, TIME_ATIME },
        { "-lut", SORT_TIME, TIME_ATIME },
        { "-gut", SORT_TIME, TIME_ATIME },
        { "-uUlC", SORT_NONE, TIME_ATIME },
        { "-luC", SORT_TIME, TIME_ATIME },
        { "-tU", SORT_NONE, TIME_MTIME },
        { "-Ut", SORT_TIME, TIME_MTIME },
        { "-tfu", SORT_NONE, TIME_ATIME },
        { "-fut", SORT_TIME, TIME_ATIME }
    };

    for (unsigned int i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
    {
        const char *arguments[] = { cases[i].flags, NULL };
        const t_parsed_options result = options_parse(arguments, 0);

        CU_ASSERT_EQUAL(result.invalid_option, '\0');
        CU_ASSERT_EQUAL(result.options.sort_type, cases[i].sort_type);
        CU_ASSERT_EQUAL(result.options.time_type, cases[i].time_type);
    }
}

static void should_apply_f_settings_including_when_sort_is_overridden(void)
{
    const char *arguments[] = { "-lf", "--color", "-t", NULL };
    const char *f_arguments[] = { "-f", NULL };
    const t_parsed_options result = options_parse(arguments, 0);
    const t_parsed_options f_result = options_parse(f_arguments, 0);

    CU_ASSERT_EQUAL(result.invalid_option, '\0');
    CU_ASSERT_EQUAL(result.options.format, FORMAT_LONG);
    CU_ASSERT_EQUAL(result.options.sort_type, SORT_TIME);
    CU_ASSERT_TRUE(result.options.color_files);
    CU_ASSERT_FALSE(result.options.filter_hidden_files);
    CU_ASSERT_EQUAL(f_result.invalid_option, '\0');
    CU_ASSERT_EQUAL(f_result.options.format, FORMAT_SINGLE_COLUMN);
    CU_ASSERT_EQUAL(f_result.options.sort_type, SORT_NONE);
    CU_ASSERT_FALSE(f_result.options.filter_hidden_files);
}

static void should_disable_recursion_and_dereferencing_for_immediate_directories(void)
{
    const char *first[] = { "-d", "-RC", NULL };
    const char *last[] = { "-Rd", NULL };
    const t_parsed_options first_result = options_parse(first, 0);
    const t_parsed_options last_result = options_parse(last, 0);

    CU_ASSERT_EQUAL(first_result.invalid_option, '\0');
    CU_ASSERT_TRUE(first_result.options.immediate_dirs);
    CU_ASSERT_FALSE(first_result.options.recursion);
    CU_ASSERT_FALSE(first_result.options.dereference_symlinks);
    CU_ASSERT_EQUAL(last_result.invalid_option, '\0');
    CU_ASSERT_TRUE(last_result.options.immediate_dirs);
    CU_ASSERT_FALSE(last_result.options.recursion);
    CU_ASSERT_FALSE(last_result.options.dereference_symlinks);
}

static void should_count_only_the_option_when_a_lone_dash_is_also_passed(void)
{
    const char *arguments[] = { "-", "-l", NULL };
    const t_parsed_options result = options_parse(arguments, 0);

    CU_ASSERT_EQUAL(result.invalid_option, '\0');
    CU_ASSERT_EQUAL(result.options_amount, 1);
    CU_ASSERT_EQUAL(result.options.format, FORMAT_LONG);
}

static void should_return_the_first_unsupported_option_character(void)
{
    const char *arguments[] = { "-laX", "-z", NULL };
    const t_parsed_options result = options_parse(arguments, 0);

    CU_ASSERT_EQUAL(result.invalid_option, 'X');
}

void register_options_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, NULL, NULL);

    if (suite == NULL)
        return ;

    CU_add_test(suite, "should_resolve_defaults_without_options", should_resolve_defaults_without_options);
    CU_add_test(suite, "should_default_to_columns_on_a_terminal", should_default_to_columns_on_a_terminal);
    CU_add_test(suite, "should_accumulate_independent_settings_and_count_option_arguments", should_accumulate_independent_settings_and_count_option_arguments);
    CU_add_test(suite, "should_use_the_last_explicit_format_and_preserve_owner_preference", should_use_the_last_explicit_format_and_preserve_owner_preference);
    CU_add_test(suite, "should_resolve_explicit_and_implicit_time_sorting", should_resolve_explicit_and_implicit_time_sorting);
    CU_add_test(suite, "should_apply_f_settings_including_when_sort_is_overridden", should_apply_f_settings_including_when_sort_is_overridden);
    CU_add_test(suite, "should_disable_recursion_and_dereferencing_for_immediate_directories", should_disable_recursion_and_dereferencing_for_immediate_directories);
    CU_add_test(suite, "should_count_only_the_option_when_a_lone_dash_is_also_passed", should_count_only_the_option_when_a_lone_dash_is_also_passed);
    CU_add_test(suite, "should_return_the_first_unsupported_option_character", should_return_the_first_unsupported_option_character);
}
