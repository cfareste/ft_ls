#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "sorter.h"

#define SUITE_NAME "sorter"

static void should_not_fail_when_passing_a_NULL_pointer(void)
{
    sort(NULL);
}

static void should_not_fail_when_passing_a_NULL_file_entry_list(void)
{
    t_file_entry_list *invalid_list = NULL;

    sort(&invalid_list);
}

static void should_do_nothing_when_a_file_entry_list_with_one_entry_is_passed(void)
{
    t_file_entry_list *file_entry_list = file_entry_list_create("file");

    sort(&file_entry_list);

    CU_ASSERT_STRING_EQUAL(file_entry_list_get_name(file_entry_list), "file");
    CU_ASSERT_EQUAL(file_entry_list_get_length(file_entry_list), 1);

    file_entry_list_destroy(&file_entry_list);
}

static void should_do_nothing_when_an_already_sorted_file_entry_list_is_passed(void)
{
    t_file_entry_list *file_entry_list = file_entry_list_create("file");
    file_entry_list_add_entry(&file_entry_list, file_entry_list_create("file2"));
    file_entry_list_add_entry(&file_entry_list, file_entry_list_create("file3"));
    file_entry_list_add_entry(&file_entry_list, file_entry_list_create("file4"));

    sort(&file_entry_list);
    const t_file_entry_list *second_entry = file_entry_list_get_next(file_entry_list);
    const t_file_entry_list *third_entry = file_entry_list_get_next(second_entry);
    const t_file_entry_list *fourth_entry = file_entry_list_get_next(third_entry);

    CU_ASSERT_STRING_EQUAL(file_entry_list_get_name(file_entry_list), "file");
    CU_ASSERT_STRING_EQUAL(file_entry_list_get_name(second_entry), "file2");
    CU_ASSERT_STRING_EQUAL(file_entry_list_get_name(third_entry), "file3");
    CU_ASSERT_STRING_EQUAL(file_entry_list_get_name(fourth_entry), "file4");
    CU_ASSERT_EQUAL(file_entry_list_get_length(file_entry_list), 4);

    file_entry_list_destroy(&file_entry_list);
}

void register_sorter_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, NULL, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_not_fail_when_passing_a_NULL_pointer", should_not_fail_when_passing_a_NULL_pointer);
        CU_add_test(suite, "should_not_fail_when_passing_a_NULL_file_entry_list", should_not_fail_when_passing_a_NULL_file_entry_list);
        CU_add_test(suite, "should_do_nothing_when_a_file_entry_list_with_one_entry_is_passed", should_do_nothing_when_a_file_entry_list_with_one_entry_is_passed);
        CU_add_test(suite, "should_do_nothing_when_an_already_sorted_file_entry_list_is_passed", should_do_nothing_when_an_already_sorted_file_entry_list_is_passed);
    }
}
