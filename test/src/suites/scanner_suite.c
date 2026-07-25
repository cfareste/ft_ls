#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#include "scanner.h"

#define SUITE_NAME "scanner"

static t_file_data *file_data;

static void scan_directory(const char *path)
{
    file_data = scan(path);
}

static void assertFileDataIsNull()
{
    CU_ASSERT_PTR_NULL(file_data);
}

static void should_return_NULL_if_a_NULL_path_is_specified(void)
{
    scan_directory(NULL);

    assertFileDataIsNull();
}

static void should_return_NULL_if_an_empty_path_is_specified(void)
{
    scan_directory("");

    assertFileDataIsNull();
}

static void should_return_NULL_if_the_current_directory_is_empty(void)
{
    scan_directory(".");

    assertFileDataIsNull();
}

static void should_return_one_entry_if_the_current_directory_has_one_file(void)
{
    scan_directory(".");

    int count = 0;
    for (t_file_data *ptr = file_data; ptr != NULL; ptr = file_data_get_next(ptr))
    {
        count++;
    }
    CU_ASSERT_EQUAL(count, 0);
    CU_ASSERT_STRING_EQUAL(file_data_get_name(file_data), "file");
}

void register_scanner_suite(void)
{
    const CU_pSuite suite = CU_add_suite(SUITE_NAME, NULL, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_return_NULL_if_a_NULL_path_is_specified", should_return_NULL_if_a_NULL_path_is_specified);
        CU_add_test(suite, "should_return_NULL_if_an_empty_path_is_specified", should_return_NULL_if_an_empty_path_is_specified);
        CU_add_test(suite, "should_return_NULL_if_the_current_directory_is_empty", should_return_NULL_if_the_current_directory_is_empty);
        CU_add_test(suite, "should_return_one_entry_if_the_current_directory_has_one_file", should_return_one_entry_if_the_current_directory_has_one_file);
    }
}
