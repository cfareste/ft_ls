#include "file_data.h"

#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#define SUITE_NAME "file_data"

static void x(void)
{
    CU_ASSERT_TRUE(1);
}

void register_file_data_suite(void)
{
    const CU_pSuite suite = CU_add_suite(SUITE_NAME, NULL, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "x", x);
    }
}
