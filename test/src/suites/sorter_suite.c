#include "suites.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "sorter.h"
#include "parsed_arguments.h"
#include "libft.h"

#define SUITE_NAME "sorter"

static int compare_by_ascii(const void *first_string, const void *second_string)
{
    return ft_strcmp(first_string, second_string);
}

static void should_not_fail_when_passing_a_NULL_pointer(void)
{
    sort_pointer_array(NULL, NULL, compare_by_ascii);
}

static void should_not_fail_when_passing_a_NULL_file_entry_array(void)
{
    void *invalid_array = NULL;

    sort_pointer_array(NULL, &invalid_array, compare_by_ascii);
}

static void should_not_fail_when_passing_a_NULL_comparator_function(void)
{
    void *array[] = { "valid", NULL };

    sort_pointer_array(NULL, array, NULL);
}

static void should_do_nothing_when_a_file_entry_array_with_one_entry_is_passed(void)
{
    void *array[] = { "file", NULL };

    sort_pointer_array(NULL, array, compare_by_ascii);

    CU_ASSERT_STRING_EQUAL(array[0], "file");
    CU_ASSERT_PTR_NULL(array[1]);
}

static void should_do_nothing_when_an_already_sorted_file_entry_array_is_passed(void)
{
    void *array[] = { ".hidden", "2file", "FILE", "_file", "a", "f", "file", NULL };

    sort_pointer_array(NULL, array, compare_by_ascii);

    CU_ASSERT_STRING_EQUAL(array[0], ".hidden");
    CU_ASSERT_STRING_EQUAL(array[1], "2file");
    CU_ASSERT_STRING_EQUAL(array[2], "FILE");
    CU_ASSERT_STRING_EQUAL(array[3], "_file");
    CU_ASSERT_STRING_EQUAL(array[4], "a");
    CU_ASSERT_STRING_EQUAL(array[5], "f");
    CU_ASSERT_STRING_EQUAL(array[6], "file");
    CU_ASSERT_PTR_NULL(array[7]);
}

static void should_sort_an_unsorted_array(void)
{
    void *array[] = { "a", "2file", ".hidden", "_file", "f", "file", "FILE", NULL };

    sort_pointer_array(NULL, array, compare_by_ascii);

    CU_ASSERT_STRING_EQUAL(array[0], ".hidden");
    CU_ASSERT_STRING_EQUAL(array[1], "2file");
    CU_ASSERT_STRING_EQUAL(array[2], "FILE");
    CU_ASSERT_STRING_EQUAL(array[3], "_file");
    CU_ASSERT_STRING_EQUAL(array[4], "a");
    CU_ASSERT_STRING_EQUAL(array[5], "f");
    CU_ASSERT_STRING_EQUAL(array[6], "file");
    CU_ASSERT_PTR_NULL(array[7]);
}

static void should_sort_an_unsorted_array_in_reverse_order_when_reverse_option_is_active(void)
{
    const char *arguments[] = { "-r", NULL };
    t_result *parsed_arguments_result = parse_arguments(1, arguments);
    t_parsed_arguments *parsed_arguments = result_get_value(parsed_arguments_result);
    void *array[] = { "a", "2file", ".hidden", "_file", "f", "file", "FILE", NULL };

    sort_pointer_array(parsed_arguments, array, compare_by_ascii);

    CU_ASSERT_STRING_EQUAL(array[0], "file");
    CU_ASSERT_STRING_EQUAL(array[1], "f");
    CU_ASSERT_STRING_EQUAL(array[2], "a");
    CU_ASSERT_STRING_EQUAL(array[3], "_file");
    CU_ASSERT_STRING_EQUAL(array[4], "FILE");
    CU_ASSERT_STRING_EQUAL(array[5], "2file");
    CU_ASSERT_STRING_EQUAL(array[6], ".hidden");
    CU_ASSERT_PTR_NULL(array[7]);

    parsed_arguments_destroy(&parsed_arguments);
    result_destroy(&parsed_arguments_result);
}

static void should_preserve_array_order_when_no_sort_overrides_time_and_reverse_is_requested(void)
{
    const char *arguments[] = { "-trU", NULL };
    t_result *result = parse_arguments(1, arguments);
    t_parsed_arguments *parsed_arguments = result_get_value(result);
    void *array[] = { "b", "a", "c", NULL };

    sort_pointer_array(parsed_arguments, array, compare_by_ascii);

    CU_ASSERT_TRUE(result_has_succeed(result));
    CU_ASSERT_STRING_EQUAL(array[0], "b");
    CU_ASSERT_STRING_EQUAL(array[1], "a");
    CU_ASSERT_STRING_EQUAL(array[2], "c");

    parsed_arguments_destroy(&parsed_arguments);
    result_destroy(&result);
}

void register_sorter_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, NULL, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_not_fail_when_passing_a_NULL_pointer", should_not_fail_when_passing_a_NULL_pointer);
        CU_add_test(suite, "should_not_fail_when_passing_a_NULL_file_entry_array", should_not_fail_when_passing_a_NULL_file_entry_array);
        CU_add_test(suite, "should_not_fail_when_passing_a_NULL_comparator_function", should_not_fail_when_passing_a_NULL_comparator_function);
        CU_add_test(suite, "should_do_nothing_when_a_file_entry_array_with_one_entry_is_passed", should_do_nothing_when_a_file_entry_array_with_one_entry_is_passed);
        CU_add_test(suite, "should_do_nothing_when_an_already_sorted_file_entry_array_is_passed", should_do_nothing_when_an_already_sorted_file_entry_array_is_passed);
        CU_add_test(suite, "should_sort_an_unsorted_array", should_sort_an_unsorted_array);
        CU_add_test(suite, "should_sort_an_unsorted_array_in_reverse_order_when_reverse_option_is_active", should_sort_an_unsorted_array_in_reverse_order_when_reverse_option_is_active);
        CU_add_test(suite, "should_preserve_array_order_when_no_sort_overrides_time_and_reverse_is_requested", should_preserve_array_order_when_no_sort_overrides_time_and_reverse_is_requested);
    }
}
