#include <string.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "mocks.h"
#include "scanner.h"

#define SUITE_NAME "scanner"

static t_file_data *sut;

static void test_setup(void)
{
    reset_dirent_guarantees();
}

static void test_teardown(void)
{
    file_data_destroy(&sut);
}

static void scan_directory(const char *path)
{
    sut = scan(path);
}

static void assert_file_data_is_null()
{
    CU_ASSERT_PTR_NULL(sut);
}

static void assert_file_data_length_is(const unsigned int length)
{
    CU_ASSERT_EQUAL(file_data_get_length(sut), length);
}

static void assert_file_data_name_is(const t_file_data *_file_data, const char *name)
{
    CU_ASSERT_STRING_EQUAL(file_data_get_name(_file_data), name);
}

static void should_return_NULL_if_a_NULL_path_is_specified(void)
{
    scan_directory(NULL);

    assert_file_data_is_null();
}

static void should_return_NULL_if_an_empty_path_is_specified(void)
{
    scan_directory("");

    assert_file_data_is_null();
}

static void should_return_NULL_if_the_current_directory_is_empty(void)
{
    scan_directory(".");

    assert_file_data_is_null();
}

static void should_return_one_entry_if_the_current_directory_has_one_file(void)
{
    const char *file_name = "file";
    guarantee_readdir_will_return_a_file_named(file_name);

    scan_directory(".");

    assert_file_data_length_is(1);
    assert_file_data_name_is(sut, file_name);
}

static void should_return_multiple_entries_if_the_current_directory_has_more_than_one_file(void)
{
    const char *files_names[4] = { "multiple", "multiple2", "multiple3", NULL };
    guarantee_readdir_will_return_N_files_named(files_names);

    scan_directory(".");
    const t_file_data *file_data_second = file_data_get_next(sut);
    const t_file_data *file_data_third = file_data_get_next(file_data_second);

    assert_file_data_length_is(3);
    assert_file_data_name_is(sut, files_names[0]);
    assert_file_data_name_is(file_data_second, files_names[1]);
    assert_file_data_name_is(file_data_third, files_names[2]);
}

static void should_return_NULL_if_fails_to_open_a_directory(void)
{
    guarantee_opendir_will_fail();

    scan_directory(".");

    assert_file_data_is_null();
}

static void should_return_NULL_if_fails_to_read_a_directory(void)
{
    guarantee_readdir_will_fail();

    scan_directory(".");

    assert_file_data_is_null();
}

void register_scanner_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, test_teardown);

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
