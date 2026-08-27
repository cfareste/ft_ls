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

void register_sorter_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, NULL, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_not_fail_when_passed_a_NULL_pointer", should_not_fail_when_passed_a_NULL_pointer);
        CU_add_test(suite, "should_not_fail_when_passed_a_pointer_pointing_at_NULL", should_not_fail_when_passed_a_pointer_pointing_at_NULL);
    }
}
