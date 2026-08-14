#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "application_options.h"

#define SUITE_NAME "application_options"

static t_ft_ls_options *sut;

static void test_setup()
{
    sut = ft_ls_options_get(0);
}

static void should_be_created_correctly(void)
{
    CU_ASSERT_PTR_NOT_NULL(sut);

    ft_ls_options_destroy(&sut);
}

static void should_be_destroyed_correctly(void)
{
    ft_ls_options_destroy(&sut);

    CU_ASSERT_PTR_NULL(sut);
}

static void should_return_NULL_if_the_number_of_arguments_is_negative(void)
{
    t_ft_ls_options *options = ft_ls_options_get(-1);

    CU_ASSERT_PTR_NULL(options);

    ft_ls_options_destroy(&options);
}

void register_application_options_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_be_created_correctly", should_be_created_correctly);
        CU_add_test(suite, "should_be_destroyed_correctly", should_be_destroyed_correctly);
        CU_add_test(suite, "should_return_NULL_if_the_number_of_arguments_is_negative", should_return_NULL_if_the_number_of_arguments_is_negative);
    }
}
