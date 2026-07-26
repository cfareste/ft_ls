#include <string.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "mocks.h"
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
    reset_readdir_affirmations();

    scan_directory(".");

    assertFileDataIsNull();
}

static void should_return_one_entry_if_the_current_directory_has_one_file(void)
{
    reset_readdir_affirmations();
    affirm_readdir_will_return_a_file_named("file");

    scan_directory(".");

    CU_ASSERT_EQUAL(file_data_get_length(file_data), 1);
    CU_ASSERT_STRING_EQUAL(file_data_get_name(file_data), "file");

    file_data_destroy(&file_data);
}

static void should_return_multiple_entries_if_the_current_directory_has_more_than_one_file(void)
{
    reset_readdir_affirmations();
    affirm_readdir_will_return_N_files_named("multiple", 3);

    scan_directory(".");
    const t_file_data *file_data_second = file_data_get_next(file_data);
    const t_file_data *file_data_third = file_data_get_next(file_data_second);

    CU_ASSERT_EQUAL(file_data_get_length(file_data), 3);
    CU_ASSERT_STRING_EQUAL(file_data_get_name(file_data), "multiple");
    CU_ASSERT_STRING_EQUAL(file_data_get_name(file_data_second), "multiple2");
    CU_ASSERT_STRING_EQUAL(file_data_get_name(file_data_third), "multiple3");

    file_data_destroy(&file_data);
}

static void should_return_NULL_if_fails_to_open_a_directory(void)
{
    reset_readdir_affirmations();
    affirm_readdir_will_return_a_file_named("should_fail");
    g_opendir_fail = 1;

    scan_directory(".");

    assertFileDataIsNull();

    file_data_destroy(&file_data);
}

static void should_return_NULL_if_fails_to_read_a_directory(void)
{
    reset_readdir_affirmations();
    g_opendir_fail = 0;
    g_readdir_fail = 1;

    scan_directory(".");

    assertFileDataIsNull();

    file_data_destroy(&file_data);
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
        CU_add_test(suite, "should_return_multiple_entries_if_the_current_directory_has_more_than_one_file", should_return_multiple_entries_if_the_current_directory_has_more_than_one_file);
        CU_add_test(suite, "should_return_NULL_if_fails_to_open_a_directory", should_return_NULL_if_fails_to_open_a_directory);
        CU_add_test(suite, "should_return_NULL_if_fails_to_read_a_directory", should_return_NULL_if_fails_to_read_a_directory);
    }
}
