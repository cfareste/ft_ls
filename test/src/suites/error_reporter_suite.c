#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "error_reporter.h"
#include "mocks.h"

#define SUITE_NAME "error_reporter"
#define OPEN_DIRECTORY_ERROR_PREFIX "ft_ls: cannot open directory"
#define READ_DIRECTORY_ERROR_PREFIX "ft_ls: reading directory"

static void test_setup(void)
{
    errno = 0;
    reset_printing_buffer();
}

static void should_only_print_the_open_directory_error_if_a_NULL_directory_path_is_specified(void)
{
    errno = ENOTDIR;

    report_opening_directory_error(NULL);

    CU_ASSERT(verify_that_the_error_printed_is( "%s: %s\n", OPEN_DIRECTORY_ERROR_PREFIX, strerror(errno)));
}

static void should_only_print_the_open_directory_error_if_an_empty_directory_path_is_specified(void)
{
    errno = ENOENT;

    report_opening_directory_error("");

    CU_ASSERT(verify_that_the_error_printed_is( "%s: %s\n", OPEN_DIRECTORY_ERROR_PREFIX, strerror(errno)));
}

static void should_print_the_open_directory_error_with_the_specified_directory_path(void)
{
    errno = ENFILE;
    const char *directory_path = "./path/to/directory";

    report_opening_directory_error(directory_path);

    CU_ASSERT(verify_that_the_error_printed_is("%s '%s': %s\n", OPEN_DIRECTORY_ERROR_PREFIX, directory_path, strerror(errno)));
}

static void should_only_print_the_read_directory_error_if_a_NULL_directory_path_is_specified(void)
{
    errno = EBADF;

    report_reading_directory_error(NULL);

    CU_ASSERT(verify_that_the_error_printed_is( "%s: %s\n", READ_DIRECTORY_ERROR_PREFIX, strerror(errno)));
}

static void should_only_print_the_read_directory_error_if_an_empty_directory_path_is_specified(void)
{
    errno = EBADF;

    report_reading_directory_error("");

    CU_ASSERT(verify_that_the_error_printed_is( "%s: %s\n", READ_DIRECTORY_ERROR_PREFIX, strerror(errno)));
}

void register_error_reporter_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_only_print_the_open_directory_error_if_a_NULL_directory_path_is_specified", should_only_print_the_open_directory_error_if_a_NULL_directory_path_is_specified);
        CU_add_test(suite, "should_only_print_the_open_directory_error_if_an_empty_directory_path_is_specified", should_only_print_the_open_directory_error_if_an_empty_directory_path_is_specified);
        CU_add_test(suite, "should_print_the_open_directory_error_with_the_specified_directory_path", should_print_the_open_directory_error_with_the_specified_directory_path);
        CU_add_test(suite, "should_only_print_the_read_directory_error_if_a_NULL_directory_path_is_specified", should_only_print_the_read_directory_error_if_a_NULL_directory_path_is_specified);
        CU_add_test(suite, "should_only_print_the_read_directory_error_if_an_empty_directory_path_is_specified", should_only_print_the_read_directory_error_if_an_empty_directory_path_is_specified);
    }
}
