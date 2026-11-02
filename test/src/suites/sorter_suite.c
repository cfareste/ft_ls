#include "suites.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "sorter.h"
#include "libft.h"

#define SUITE_NAME "sorter"

static t_sorter_context *sorter_context;

static void test_setup(void)
{
    sorter_context = sorter_context_create(NULL);
}

static void test_teardown(void)
{
    sorter_context_destroy(&sorter_context);
}

static int compare_by_ascii(const void *first_string, const void *second_string)
{
    return ft_strcmp(first_string, second_string);
}

static void should_create_sorter_context(void)
{
    t_sorter_context *context = sorter_context_create(NULL);

    CU_ASSERT_PTR_NOT_NULL(context);

    sorter_context_destroy(&context);
}

static void should_destroy_sorter_context(void)
{
    t_sorter_context *context = sorter_context_create(NULL);

    sorter_context_destroy(&context);
    sorter_context_destroy(NULL);

    CU_ASSERT_PTR_NULL(context);
}

static void should_not_fail_when_passing_a_NULL_pointer(void)
{
    sort_pointer_array(sorter_context, NULL, compare_by_ascii);
}

static void should_not_fail_when_passing_a_NULL_file_entry_array(void)
{
    void *invalid_array = NULL;

    sort_pointer_array(sorter_context, &invalid_array, compare_by_ascii);
}

static void should_not_fail_when_passing_a_NULL_comparator_function(void)
{
    void *array[] = { "valid", NULL };

    sort_pointer_array(sorter_context, array, NULL);
}

static void should_do_nothing_when_a_file_entry_array_with_one_entry_is_passed(void)
{
    void *array[] = { "file", NULL };

    sort_pointer_array(sorter_context, array, compare_by_ascii);

    CU_ASSERT_STRING_EQUAL(array[0], "file");
    CU_ASSERT_PTR_NULL(array[1]);
}

static void should_do_nothing_when_an_already_sorted_file_entry_array_is_passed(void)
{
    void *array[] = { ".hidden", "2file", "FILE", "_file", "a", "f", "file", NULL };

    sort_pointer_array(sorter_context, array, compare_by_ascii);

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

    sort_pointer_array(sorter_context, array, compare_by_ascii);

    CU_ASSERT_STRING_EQUAL(array[0], ".hidden");
    CU_ASSERT_STRING_EQUAL(array[1], "2file");
    CU_ASSERT_STRING_EQUAL(array[2], "FILE");
    CU_ASSERT_STRING_EQUAL(array[3], "_file");
    CU_ASSERT_STRING_EQUAL(array[4], "a");
    CU_ASSERT_STRING_EQUAL(array[5], "f");
    CU_ASSERT_STRING_EQUAL(array[6], "file");
    CU_ASSERT_PTR_NULL(array[7]);
}

void register_sorter_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, test_teardown);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_create_sorter_context", should_create_sorter_context);
        CU_add_test(suite, "should_destroy_sorter_context", should_destroy_sorter_context);
        CU_add_test(suite, "should_not_fail_when_passing_a_NULL_pointer", should_not_fail_when_passing_a_NULL_pointer);
        CU_add_test(suite, "should_not_fail_when_passing_a_NULL_file_entry_array", should_not_fail_when_passing_a_NULL_file_entry_array);
        CU_add_test(suite, "should_not_fail_when_passing_a_NULL_comparator_function", should_not_fail_when_passing_a_NULL_comparator_function);
        CU_add_test(suite, "should_do_nothing_when_a_file_entry_array_with_one_entry_is_passed", should_do_nothing_when_a_file_entry_array_with_one_entry_is_passed);
        CU_add_test(suite, "should_do_nothing_when_an_already_sorted_file_entry_array_is_passed", should_do_nothing_when_an_already_sorted_file_entry_array_is_passed);
        CU_add_test(suite, "should_sort_an_unsorted_array", should_sort_an_unsorted_array);
    }
}
