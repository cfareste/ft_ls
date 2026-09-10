#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "error_reporter.h"
#include "mocks.h"

#define SUITE_NAME "error_reporter"

static void test_setup(void)
{
    errno = 0;
    reset_printing_buffer();
}

static void should_only_print_the_application_prefix_and_system_error_if_a_NULL_message_is_specified(void)
{
    errno = ENOTDIR;

    report_error(NULL);

    CU_ASSERT(verify_that_the_error_printed_is("ft_ls: %s\n", strerror(errno)));
}

static void should_only_print_the_application_prefix_and_system_error_if_an_empty_message_is_specified(void)
{
    errno = ENOENT;

    report_error("");

    CU_ASSERT(verify_that_the_error_printed_is("ft_ls: %s\n", strerror(errno)));
}

void register_error_reporter_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_only_print_the_application_prefix_and_system_error_if_a_NULL_message_is_specified", should_only_print_the_application_prefix_and_system_error_if_a_NULL_message_is_specified);
        CU_add_test(suite, "should_only_print_the_application_prefix_and_system_error_if_an_empty_message_is_specified", should_only_print_the_application_prefix_and_system_error_if_an_empty_message_is_specified);
    }
}
