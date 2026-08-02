#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "directory.h"

#define SUITE_NAME "directory"

static DIR *dir;

static void test_teardown(void)
{
    directory_close(dir);
}

static void should_return_NULL_when_opening_a_NULL_path(void)
{
    dir = directory_open(NULL);

    CU_ASSERT_PTR_NULL(dir);
}

static void should_return_NULL_when_opening_an_empty_path(void)
{
    dir = directory_open("");

    CU_ASSERT_PTR_NULL(dir);
}

static void should_return_a_directory_stream_when_opening_a_valid_path(void)
{
    dir = directory_open("valid");

    CU_ASSERT_PTR_NOT_NULL(dir);
}

void register_directory_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL,  NULL, test_teardown);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_return_NULL_when_opening_a_NULL_path", should_return_NULL_when_opening_a_NULL_path);
        CU_add_test(suite, "should_return_NULL_when_opening_an_empty_path", should_return_NULL_when_opening_an_empty_path);
        CU_add_test(suite, "should_return_a_directory_stream_when_opening_a_valid_path", should_return_a_directory_stream_when_opening_a_valid_path);
    }
}
