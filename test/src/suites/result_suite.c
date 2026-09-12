#include <stdlib.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "result.h"

#define SUITE_NAME "result"

static void should_create_a_successful_result(void)
{
    t_result *result = result_create_successful("value");

    CU_ASSERT_STRING_EQUAL(result_get_value(result), "value");
    CU_ASSERT_EQUAL(result_has_failed(result), 0);

    result_destroy(&result);
}

static void should_create_a_failed_result(void)
{
    t_result *result = result_create_failed();

    CU_ASSERT_PTR_NULL(result_get_value(result));
    CU_ASSERT_EQUAL(result_has_failed(result), 1);

    result_destroy(&result);
}

static void should_destroy_a_result(void)
{
    t_result *result = result_create_successful("value");

    result_destroy(&result);

    CU_ASSERT_PTR_NULL(result);
}

static void should_return_NULL_for_value_if_a_NULL_result_is_passed(void)
{
    CU_ASSERT_PTR_NULL(result_get_value(NULL));
}

static void should_return_the_result_value_of_the_specified_result(void)
{
    t_result *result = result_create_successful("value");

    CU_ASSERT_STRING_EQUAL(result_get_value(result), "value");

    result_destroy(&result);
}

static void should_return_true_for_has_failed_if_a_NULL_result_is_passed(void)
{
    CU_ASSERT_EQUAL(result_has_failed(NULL), 1);
    CU_ASSERT_EQUAL(result_has_succeed(NULL), 0);
}

static void should_return_false_for_has_failed_if_a_successful_result_is_passed(void)
{
    t_result *result = result_create_successful("value");

    CU_ASSERT_STRING_EQUAL(result_get_value(result), "value");
    CU_ASSERT_EQUAL(result_has_failed(result), 0);
    CU_ASSERT_EQUAL(result_has_succeed(result), 1);

    result_destroy(&result);
}

static void should_return_true_for_has_failed_if_a_failed_result_is_passed(void)
{
    t_result *result = result_create_failed();

    CU_ASSERT_PTR_NULL(result_get_value(result));
    CU_ASSERT_EQUAL(result_has_failed(result), 1);
    CU_ASSERT_EQUAL(result_has_succeed(result), 0);

    result_destroy(&result);
}

void register_result_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, NULL, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_create_a_successful_result", should_create_a_successful_result);
        CU_add_test(suite, "should_create_a_failed_result", should_create_a_failed_result);
        CU_add_test(suite, "should_destroy_a_result", should_destroy_a_result);
        CU_add_test(suite, "should_return_NULL_for_value_if_a_NULL_result_is_passed", should_return_NULL_for_value_if_a_NULL_result_is_passed);
        CU_add_test(suite, "should_return_the_result_value_of_the_specified_result", should_return_the_result_value_of_the_specified_result);
        CU_add_test(suite, "should_return_true_for_has_failed_if_a_NULL_result_is_passed", should_return_true_for_has_failed_if_a_NULL_result_is_passed);
        CU_add_test(suite, "should_return_false_for_has_failed_if_a_successful_result_is_passed", should_return_false_for_has_failed_if_a_successful_result_is_passed);
        CU_add_test(suite, "should_return_true_for_has_failed_if_a_failed_result_is_passed", should_return_true_for_has_failed_if_a_failed_result_is_passed);
    }
}
