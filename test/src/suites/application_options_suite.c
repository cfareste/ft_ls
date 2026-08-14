#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "application_options.h"

#define SUITE_NAME "application_options"

static void should_be_created_correctly(void)
{
    t_ft_ls_options *options = ft_ls_options_get();

    CU_ASSERT_PTR_NOT_NULL(options);

    ft_ls_options_destroy(&options);
}

static void should_be_destroyed_correctly(void)
{
    t_ft_ls_options *options = ft_ls_options_get();

    ft_ls_options_destroy(&options);

    CU_ASSERT_PTR_NULL(options);
}

void register_application_options_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, NULL, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_be_created_correctly", should_be_created_correctly);
        CU_add_test(suite, "should_be_destroyed_correctly", should_be_destroyed_correctly);
    }
}
