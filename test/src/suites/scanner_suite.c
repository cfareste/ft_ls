#include <string.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "mocks.h"
#include "scanner.h"

#define SUITE_NAME "scanner"
#define CURRENT_DIRECTORY_PATH "."

static t_file_entry_list *sut;

static void test_setup(void)
{
    const char *dir_names[] = { CURRENT_DIRECTORY_PATH, NULL };
    reset_dirent_guarantees();
    reset_stat_guarantees();
    ensure_opendir_will_open_N_dirs_named(dir_names);
}

static void test_teardown(void)
{
    file_entry_list_destroy(&sut);
}

static void scan_directory(const char *path)
{
    sut = scan(path);
}

static void assert_file_entry_list_is_null()
{
    CU_ASSERT_PTR_NULL(sut);
}

static void assert_file_entry_list_length_is(const unsigned int length)
{
    CU_ASSERT_EQUAL(file_entry_list_get_length(sut), length);
}

static void assert_file_entry_list_name_is(const t_file_entry_list *file_entry_list, const char *name)
{
    CU_ASSERT_STRING_EQUAL(file_entry_list_get_name(file_entry_list), name);
}

static void assert_file_entry_list_names_are(const char **files_names)
{
    unsigned int i = 0;
    const t_file_entry_list *current = sut;
    while (current != NULL)
    {
        assert_file_entry_list_name_is(current, files_names[i]);
        current = file_entry_list_get_next(current);
        i++;
    }
}

static void should_return_NULL_if_a_NULL_path_is_specified(void)
{
    scan_directory(NULL);

    assert_file_entry_list_is_null();
}

static void should_return_NULL_if_an_empty_path_is_specified(void)
{
    scan_directory("");

    assert_file_entry_list_is_null();
}

static void should_return_NULL_if_the_current_directory_is_empty(void)
{
    guarantee_stat_will_populate_stats_of_a_directory_type_file(CURRENT_DIRECTORY_PATH);

    scan_directory(CURRENT_DIRECTORY_PATH);

    assert_file_entry_list_is_null();
}

static void should_return_one_entry_if_the_current_directory_has_one_file(void)
{
    const char *file_names[] = { "file", NULL };
    const char **entry_names[] = { file_names, NULL };
    guarantee_stat_will_populate_stats_of_a_directory_type_file(CURRENT_DIRECTORY_PATH);
    guarantee_readdir_will_return_N_files_named(entry_names);

    scan_directory(CURRENT_DIRECTORY_PATH);

    assert_file_entry_list_length_is(1);
    assert_file_entry_list_name_is(sut, file_names[0]);
}

static void should_return_multiple_entries_if_the_current_directory_has_more_than_one_file(void)
{
    const char *files_names[4] = { "multiple", "multiple2", "multiple3", NULL };
    const char **entry_names[] = { files_names, NULL };
    guarantee_stat_will_populate_stats_of_a_directory_type_file(CURRENT_DIRECTORY_PATH);
    guarantee_readdir_will_return_N_files_named(entry_names);

    scan_directory(CURRENT_DIRECTORY_PATH);

    assert_file_entry_list_length_is(3);
    assert_file_entry_list_names_are(files_names);
}

static void should_return_a_list_of_entries_if_one_non_empty_directory_path_is_specified(void)
{
    const char *dir_names[] = { "valid_dir", NULL };
    const char *files_names[5] = { "file", "subdir", "file2", "subdir2", NULL };
    const char **entry_names[] = { files_names, NULL };
    guarantee_stat_will_populate_stats_of_a_directory_type_file(dir_names[0]);
    ensure_opendir_will_open_N_dirs_named(dir_names);
    guarantee_readdir_will_return_N_files_named(entry_names);

    scan_directory(dir_names[0]);

    assert_file_entry_list_length_is(4);
    assert_file_entry_list_names_are(files_names);
}

static void should_return_NULL_if_fails_to_open_a_directory(void)
{
    guarantee_stat_will_populate_stats_of_a_directory_type_file(CURRENT_DIRECTORY_PATH);
    guarantee_opendir_will_fail();

    scan_directory(CURRENT_DIRECTORY_PATH);

    assert_file_entry_list_is_null();
}

static void should_return_NULL_if_fails_to_read_a_directory(void)
{
    guarantee_stat_will_populate_stats_of_a_directory_type_file(CURRENT_DIRECTORY_PATH);
    guarantee_readdir_will_fail();

    scan_directory(CURRENT_DIRECTORY_PATH);

    assert_file_entry_list_is_null();
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
        CU_add_test(suite, "should_return_a_list_of_entries_if_one_non_empty_directory_path_is_specified", should_return_a_list_of_entries_if_one_non_empty_directory_path_is_specified);
        CU_add_test(suite, "should_return_NULL_if_fails_to_open_a_directory", should_return_NULL_if_fails_to_open_a_directory);
        CU_add_test(suite, "should_return_NULL_if_fails_to_read_a_directory", should_return_NULL_if_fails_to_read_a_directory);
    }
}
