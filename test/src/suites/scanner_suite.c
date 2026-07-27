#include <string.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "mocks.h"
#include "scanner.h"

#define SUITE_NAME "scanner"

static t_file_data *sut;

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
    reset_readdir_affirmations();

    scan_directory(".");

    assert_file_data_is_null();
}

static void should_return_one_entry_if_the_current_directory_has_one_file(void)
{
    reset_readdir_affirmations();
    affirm_readdir_will_return_a_file_named("file");

    scan_directory(".");

    assert_file_data_length_is(1);
    assert_file_data_name_is(sut, "file");

    file_data_destroy(&sut);
}

static void should_return_multiple_entries_if_the_current_directory_has_more_than_one_file(void)
{
    reset_readdir_affirmations();
    affirm_readdir_will_return_N_files_named("multiple", 3);

    scan_directory(".");
    const t_file_data *file_data_second = file_data_get_next(sut);
    const t_file_data *file_data_third = file_data_get_next(file_data_second);

    assert_file_data_length_is(3);
    assert_file_data_name_is(sut, "multiple");
    assert_file_data_name_is(file_data_second, "multiple2");
    assert_file_data_name_is(file_data_third, "multiple3");

    file_data_destroy(&sut);
}

static void should_return_NULL_if_fails_to_open_a_directory(void)
{
    reset_readdir_affirmations();
    affirm_readdir_will_return_a_file_named("should_fail");
    g_opendir_fail = 1;

    scan_directory(".");

    assert_file_data_is_null();

    file_data_destroy(&sut);
}

static void should_return_NULL_if_fails_to_read_a_directory(void)
{
    reset_readdir_affirmations();
    g_opendir_fail = 0;
    g_readdir_fail = 1;

    scan_directory(".");

    assert_file_data_is_null();

    file_data_destroy(&sut);
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
