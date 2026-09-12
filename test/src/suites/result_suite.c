#include <stdlib.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "libft.h"
#include "result.h"

#define SUITE_NAME "result"

static void should_create_a_successful_result(void)
{
    char *value = ft_strdup("value");
    t_result *result = result_create_successful(value);

    CU_ASSERT_STRING_EQUAL(result_get_value(result), value);
    CU_ASSERT_EQUAL(result_has_failed(result), 0);

    result_destroy(&result);
    free(value);
}

static void should_create_a_failed_result(void)
{
    t_result *result = result_create_failed();

    CU_ASSERT_PTR_NULL(result_get_value(result));
    CU_ASSERT_EQUAL(result_has_failed(result), 1);

    result_destroy(&result);
}

static void should_return_false_for_has_failed_if_a_successful_result_is_passed(void)
{
    t_result *result = result_create_successful("value");

    CU_ASSERT_STRING_EQUAL(result_get_value(result), "value");
    CU_ASSERT_EQUAL(result_has_failed(result), 0);
    CU_ASSERT_EQUAL(result_has_succeed(result), 1);

    result_destroy(&result);
}

void register_result_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, NULL, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_create_a_successful_result", should_create_a_successful_result);
        CU_add_test(suite, "should_create_a_failed_result", should_create_a_failed_result);
        CU_add_test(suite, "should_return_false_for_has_failed_if_a_successful_result_is_passed", should_return_false_for_has_failed_if_a_successful_result_is_passed);
    }
}
