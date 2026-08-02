#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "renderer.h"
#include "mocks.h"

#define SUITE_NAME "renderer"

static void should_not_print_anything_if_the_file_list_is_null(void)
{
    render(NULL);

    verify_that_the_str_that_has_been_printed_is("");
}

void register_renderer_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, NULL, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_not_print_anything_if_the_file_list_is_null", should_not_print_anything_if_the_file_list_is_null);
    }
}
