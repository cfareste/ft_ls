#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "sorter.h"

#define SUITE_NAME "sorter"

static void should_not_fail_when_passed_a_NULL_pointer(void)
{
    sort(NULL);
}

static void should_not_fail_when_passed_a_pointer_pointing_at_NULL(void)
{
    t_file_entry_list *invalid_list = NULL;

    sort(&invalid_list);
}

static void should_not_sort_when_a_list_with_one_item_is_passed(void)
{
    t_file_entry_list *file_entry_list = file_entry_list_create("file");

    sort(&file_entry_list);

    CU_ASSERT_STRING_EQUAL(file_entry_list_get_name(file_entry_list), "file");
    CU_ASSERT_EQUAL(file_entry_list_get_length(file_entry_list), 1);

    file_entry_list_destroy(&file_entry_list);
}

void register_sorter_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, NULL, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_not_fail_when_passed_a_NULL_pointer", should_not_fail_when_passed_a_NULL_pointer);
        CU_add_test(suite, "should_not_fail_when_passed_a_pointer_pointing_at_NULL", should_not_fail_when_passed_a_pointer_pointing_at_NULL);
        CU_add_test(suite, "should_not_sort_when_a_list_with_one_item_is_passed", should_not_sort_when_a_list_with_one_item_is_passed);
    }
}
