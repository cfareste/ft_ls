#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "mocks.h"

#define SUITE_NAME "file_data"

static void test_setup(void)
{
    reset_stat_guarantees();
}

void register_file_data_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, NULL);

    if (suite != NULL)
    {

    }
}
