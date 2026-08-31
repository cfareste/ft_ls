#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "sorter.h"
#include "libft.h"

#define SUITE_NAME "sorter"

static int compare_by_ascii(const void *first_string, const void *second_string)
{
    return ft_strcmp(first_string, second_string);
}

static void should_not_fail_when_passing_a_NULL_pointer(void)
{
    sort_pointer_array(NULL, compare_by_ascii);
}

static void should_not_fail_when_passing_a_NULL_file_entry_array(void)
{
    void *invalid_array = NULL;

    sort_pointer_array(&invalid_array, compare_by_ascii);
}

static void should_do_nothing_when_a_file_entry_array_with_one_entry_is_passed(void)
{
    void *array[] = { "file", NULL };

    sort_pointer_array(array, compare_by_ascii);

    CU_ASSERT_STRING_EQUAL(array[0], "file");
    CU_ASSERT_PTR_NULL(array[1]);
}

static void should_do_nothing_when_an_already_sorted_file_entry_array_is_passed(void)
{
    void *array[] = { "file", "_file", "f", "a", "2file", "FILE", NULL };

    sort_pointer_array(array, compare_by_ascii);

    CU_ASSERT_STRING_EQUAL(array[0], "file");
    CU_ASSERT_STRING_EQUAL(array[1], "_file");
    CU_ASSERT_STRING_EQUAL(array[2], "f");
    CU_ASSERT_STRING_EQUAL(array[3], "a");
    CU_ASSERT_STRING_EQUAL(array[4], "2file");
    CU_ASSERT_STRING_EQUAL(array[5], "FILE");
    CU_ASSERT_PTR_NULL(array[6]);
}

void register_sorter_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, NULL, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_not_fail_when_passing_a_NULL_pointer", should_not_fail_when_passing_a_NULL_pointer);
        CU_add_test(suite, "should_not_fail_when_passing_a_NULL_file_entry_array", should_not_fail_when_passing_a_NULL_file_entry_array);
        CU_add_test(suite, "should_do_nothing_when_a_file_entry_array_with_one_entry_is_passed", should_do_nothing_when_a_file_entry_array_with_one_entry_is_passed);
        CU_add_test(suite, "should_do_nothing_when_an_already_sorted_file_entry_array_is_passed", should_do_nothing_when_an_already_sorted_file_entry_array_is_passed);
    }
}
