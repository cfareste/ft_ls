#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "application_options.h"

#define SUITE_NAME "application_options"

static t_ft_ls_options *sut;

static void test_setup(void)
{
    const char *valid_args[] = { NULL };
    sut = ft_ls_options_get(0, valid_args);
}

static void test_teardown(void)
{
    ft_ls_options_destroy(&sut);
}

static void should_be_created_correctly(void)
{
    CU_ASSERT_PTR_NOT_NULL(sut);
}

static void should_be_destroyed_correctly(void)
{
    ft_ls_options_destroy(&sut);

    CU_ASSERT_PTR_NULL(sut);
}

static void should_not_fail_to_destroy_when_a_null_pointer_is_passed(void)
{
    ft_ls_options_destroy(NULL);
}

static void should_not_fail_to_destroy_if_its_already_null(void)
{
    ft_ls_options_destroy(&sut);
    ft_ls_options_destroy(&sut);
}

static void should_return_NULL_if_num_of_arguments_is_negative(void)
{
    const char *args[] = { "valid", "args", NULL };
    t_ft_ls_options *options = ft_ls_options_get(-1, args);

    CU_ASSERT_PTR_NULL(options);

    ft_ls_options_destroy(&options);
}

static void should_return_NULL_if_arguments_are_NULL(void)
{
    t_ft_ls_options *options = ft_ls_options_get(1, NULL);

    CU_ASSERT_PTR_NULL(options);

    ft_ls_options_destroy(&options);
}

static void should_return_the_file_parameters(void)
{
    const char *args[] = { "valid", "file", "parameters", NULL };
    t_ft_ls_options *options = ft_ls_options_get(3, args);

    const char **file_parameters = ft_ls_options_get_file_parameters(options);

    CU_ASSERT_STRING_EQUAL(file_parameters[0], args[0]);
    CU_ASSERT_STRING_EQUAL(file_parameters[1], args[1]);
    CU_ASSERT_STRING_EQUAL(file_parameters[2], args[2]);
    CU_ASSERT_PTR_NULL(file_parameters[3]);

    ft_ls_options_destroy(&options);
}

void register_application_options_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, test_teardown);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_be_created_correctly", should_be_created_correctly);
        CU_add_test(suite, "should_be_destroyed_correctly", should_be_destroyed_correctly);
        CU_add_test(suite, "should_not_fail_to_destroy_when_a_null_pointer_is_passed", should_not_fail_to_destroy_when_a_null_pointer_is_passed);
        CU_add_test(suite, "should_not_fail_to_destroy_if_its_already_null", should_not_fail_to_destroy_if_its_already_null);
        CU_add_test(suite, "should_return_NULL_if_num_of_arguments_is_negative", should_return_NULL_if_num_of_arguments_is_negative);
        CU_add_test(suite, "should_return_NULL_if_arguments_are_NULL", should_return_NULL_if_arguments_are_NULL);
        CU_add_test(suite, "should_return_the_file_parameters", should_return_the_file_parameters);
    }
}
