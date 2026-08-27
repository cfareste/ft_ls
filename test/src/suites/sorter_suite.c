#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "sorter.h"

#define SUITE_NAME "sorter"

static void should_not_fail_when_passed_a_NULL_pointer(void)
{
    sort(NULL);
}

void register_sorter_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, NULL, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_not_fail_when_passed_a_NULL_pointer", should_not_fail_when_passed_a_NULL_pointer);
    }
}
