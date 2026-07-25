#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#include "scanner.h"

#define SUITE_NAME "scanner"

static void should_return_NULL_if_a_NULL_path_is_specified(void)
{
    const t_file_data *file_data = scan(NULL);

    CU_ASSERT_PTR_NULL(file_data);
}

static void should_return_NULL_if_an_empty_path_is_specified(void)
{
    const t_file_data *file_data = scan("");

    CU_ASSERT_PTR_NULL(file_data);
}

static void should_return_NULL_if_the_current_directory_is_empty(void)
{
    const t_file_data *file_data = scan(".");

    CU_ASSERT_PTR_NULL(file_data);
}

void register_scanner_suite(void)
{
    const CU_pSuite suite = CU_add_suite(SUITE_NAME, NULL, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_return_NULL_if_a_NULL_path_is_specified", should_return_NULL_if_a_NULL_path_is_specified);
        CU_add_test(suite, "should_return_NULL_if_an_empty_path_is_specified", should_return_NULL_if_an_empty_path_is_specified);
        CU_add_test(suite, "should_return_NULL_if_the_current_directory_is_empty", should_return_NULL_if_the_current_directory_is_empty);
    }
}
