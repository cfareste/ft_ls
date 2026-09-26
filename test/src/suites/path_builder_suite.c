#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "path_builder.h"

#define SUITE_NAME "path_builder"

static void should_return_NULL_if_directory_path_is_NULL(void)
{
    const char *path = build_path(NULL, "valid_path");

    CU_ASSERT_PTR_NULL(path);
}

void register_path_builder_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, NULL, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_return_NULL_if_directory_path_is_NULL", should_return_NULL_if_directory_path_is_NULL);
    }
}
