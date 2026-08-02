#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#define SUITE_NAME "directory"

static void test_setup(void)
{

}

static void test_teardown(void)
{

}

void register_directory_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, test_teardown);

    if (suite != NULL)
    {
    }
}
