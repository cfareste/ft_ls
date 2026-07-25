#include <stdlib.h>

#include "file_data.h"

#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#define SUITE_NAME "file_data"

static void should_be_created_correctly(void)
{
    t_file_data *file_data = file_data_create();

    CU_ASSERT_PTR_NOT_NULL(file_data);

    free(file_data);
}

void register_file_data_suite(void)
{
    const CU_pSuite suite = CU_add_suite(SUITE_NAME, NULL, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_be_created_correctly", should_be_created_correctly);
    }
}
