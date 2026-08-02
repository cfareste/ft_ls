#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "directory.h"

#define SUITE_NAME "directory"

static void test_setup(void)
{

}

static void test_teardown(void)
{

}

static void should_return_NULL_when_opening_a_NULL_path(void)
{
    const DIR *dir = directory_open(NULL);

    CU_ASSERT_PTR_NULL(dir);
}

void register_directory_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, test_teardown);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_return_NULL_when_opening_a_NULL_path", should_return_NULL_when_opening_a_NULL_path);
    }
}
