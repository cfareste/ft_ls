#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#define SUITE_NAME "sorter"

static void example_test(void)
{
    CU_ASSERT_EQUAL(1, 1);
}

void register_sorter_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, NULL, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "example_test", example_test);
    }
}
