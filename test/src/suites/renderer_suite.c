#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#define SUITE_NAME "renderer"

void register_renderer_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, NULL, NULL);

    if (suite != NULL)
    {
    }
}
