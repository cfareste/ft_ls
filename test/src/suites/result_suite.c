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

void register_result_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, NULL, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_create_a_succeed_result", should_create_a_succeed_result);
    }
}
