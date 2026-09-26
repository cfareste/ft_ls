#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "path_builder.h"

#define SUITE_NAME "path_builder"

void register_path_builder_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, NULL, NULL);

    if (suite != NULL)
    {
    }
}
