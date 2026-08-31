#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "sorter.h"
#include "file_entry.h"
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
    t_file_entry_array *file_entry_array = file_entry_array_create();
    t_file_entry *file_entry = file_entry_create("file");
    file_entry_array_push(file_entry_array, file_entry);

    sort_pointer_array((void **) &file_entry_array, compare_by_ascii);

    CU_ASSERT_STRING_EQUAL(file_entry_get_name(file_entry_array_get_at(file_entry_array, 0)), "file");
    CU_ASSERT_EQUAL(file_entry_array_get_length(file_entry_array), 1);

    file_entry_array_destroy(&file_entry_array);
}

static void should_do_nothing_when_an_already_sorted_file_entry_array_is_passed(void)
{
    t_file_entry_array *file_entry_array = file_entry_array_create();
    file_entry_array_push(file_entry_array, file_entry_create("file"));
    file_entry_array_push(file_entry_array, file_entry_create("file2"));
    file_entry_array_push(file_entry_array, file_entry_create("file3"));
    file_entry_array_push(file_entry_array, file_entry_create("file4"));

    sort_pointer_array((void **) &file_entry_array, compare_by_ascii);
    const t_file_entry *first_entry = file_entry_array_get_at(file_entry_array, 0);
    const t_file_entry *second_entry = file_entry_array_get_at(file_entry_array, 1);
    const t_file_entry *third_entry = file_entry_array_get_at(file_entry_array, 2);
    const t_file_entry *fourth_entry = file_entry_array_get_at(file_entry_array, 3);

    CU_ASSERT_STRING_EQUAL(file_entry_get_name(first_entry), "file");
    CU_ASSERT_STRING_EQUAL(file_entry_get_name(second_entry), "file2");
    CU_ASSERT_STRING_EQUAL(file_entry_get_name(third_entry), "file3");
    CU_ASSERT_STRING_EQUAL(file_entry_get_name(fourth_entry), "file4");
    CU_ASSERT_EQUAL(file_entry_array_get_length(file_entry_array), 4);

    file_entry_array_destroy(&file_entry_array);
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
