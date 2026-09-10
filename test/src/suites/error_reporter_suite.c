#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#define SUITE_NAME "error_reporter"

void register_error_reporter_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, NULL, NULL);

    if (suite != NULL)
    {
    }
}
