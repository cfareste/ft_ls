#include <string.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "mocks.h"
#include "scanner.h"

#define SUITE_NAME "scanner"
#define CURRENT_DIRECTORY_PATH "."

static t_file_entry_array *sut;

static void test_setup(void)
{
    const char *dir_names[] = { CURRENT_DIRECTORY_PATH, NULL };
    vfs_mock_reset();
    reset_dirent_guarantees();
    reset_stat_guarantees();
    ensure_opendir_will_open_N_dirs_named(dir_names);
}

static void test_teardown(void)
{
    file_entry_array_destroy(&sut);
}

static void scan_directory(const char *path)
{
    sut = scan(path);
}

static void assert_file_entry_array_is_null(void)
{
    CU_ASSERT_PTR_NULL(sut);
}

static void assert_file_entry_array_length_is(const unsigned int length)
{
    CU_ASSERT_EQUAL(file_entry_array_get_length(sut), length);
}

static void assert_file_entry_name_is(const t_file_entry *file_entry, const char *name)
{
    CU_ASSERT_STRING_EQUAL(file_entry_get_name(file_entry), name);
}

static void assert_file_entry_array_names_are(const char **files_names)
{
    unsigned int i = 0;
    const unsigned int count = file_entry_array_get_length(sut);

    while (i < count)
    {
        const t_file_entry *file_entry = file_entry_array_get_at(sut, i);
        assert_file_entry_name_is(file_entry, files_names[i]);
        i++;
    }

    CU_ASSERT_PTR_NULL(files_names[i]);
}

static void should_return_NULL_if_a_NULL_path_is_specified(void)
{
    scan_directory(NULL);

    assert_file_entry_array_is_null();
}

static void should_return_NULL_if_an_empty_path_is_specified(void)
{
    scan_directory("");

    assert_file_entry_array_is_null();
}

static void should_return_one_entry_if_the_current_directory_has_one_file(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(CURRENT_DIRECTORY_PATH, ".", "..", "file"),
        MOCK_FILE("./file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    scan_directory(CURRENT_DIRECTORY_PATH);
    const t_file_entry *file_entry = file_entry_array_get_at(sut, 0);

    assert_file_entry_array_length_is(1);
    assert_file_entry_name_is(file_entry, "file");
}

static void should_return_multiple_entries_if_the_current_directory_has_more_than_one_file(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(CURRENT_DIRECTORY_PATH, ".", "..", "multiple", "multiple2", "multiple3"),
        MOCK_FILE("./multiple"),
        MOCK_FILE("./multiple2"),
        MOCK_FILE("./multiple3"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *expected_files_names[] = { "multiple", "multiple2", "multiple3", NULL };

    scan_directory(CURRENT_DIRECTORY_PATH);

    assert_file_entry_array_length_is(3);
    assert_file_entry_array_names_are(expected_files_names);
}

static void should_return_an_array_of_entries_if_one_non_empty_directory_path_is_specified(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("valid_dir", ".", "..", "file", "subdir", "file2", "subdir2"),
        MOCK_FILE("valid_dir/file"),
        MOCK_DIR("valid_dir/subdir", ".", ".."),
        MOCK_FILE("valid_dir/file2"),
        MOCK_DIR("valid_dir/subdir2", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *expected_files_names[] = { "file", "subdir", "file2", "subdir2", NULL };

    scan_directory("valid_dir");

    assert_file_entry_array_length_is(4);
    assert_file_entry_array_names_are(expected_files_names);
}

static void should_return_an_array_of_entries_without_hidden_files_if_a_directory_with_hidden_files_is_specified(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "subdir", "..", "subdir2", "file", ".gitignore"),
        MOCK_FILE("dir/file"),
        MOCK_FILE("dir/.gitignore"),
        MOCK_DIR("dir/subdir", ".", ".."),
        MOCK_DIR("dir/subdir2", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *expected_file_names[] = { "subdir", "subdir2", "file", NULL };

    scan_directory("dir");

    assert_file_entry_array_length_is(3);
    assert_file_entry_array_names_are(expected_file_names);
}

static void should_return_an_array_of_entries_if_a_hidden_directory_is_specified(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".dir", ".", "file", "..", "subdir1"),
        MOCK_FILE(".dir/file"),
        MOCK_DIR(".dir/subdir1", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *expected_file_names[] = { "file", "subdir1", NULL };

    scan_directory(".dir");

    assert_file_entry_array_length_is(2);
    assert_file_entry_array_names_are(expected_file_names);
}

static void should_return_an_empty_array_if_the_specified_directory_is_empty(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(CURRENT_DIRECTORY_PATH, ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    scan_directory(CURRENT_DIRECTORY_PATH);

    assert_file_entry_array_length_is(0);
}

static void should_return_an_empty_array_if_the_specified_directory_only_contains_hidden_files(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".gitignore", ".", ".idea/", "..", ".run"),
        MOCK_FILE("dir/.gitignore"),
        MOCK_DIR("dir/.idea", "..", "."),
        MOCK_DIR("dir/.run", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    scan_directory("dir");

    assert_file_entry_array_length_is(0);
}

// static void should_return_NULL_if_fails_to_open_a_directory(void)
// {
//     guarantee_stat_will_populate_stats_of_a_directory_type_file(CURRENT_DIRECTORY_PATH);
//     guarantee_opendir_will_fail();
//
//     scan_directory(CURRENT_DIRECTORY_PATH);
//
//     assert_file_entry_array_is_null();
// }

// static void should_return_NULL_if_fails_to_read_a_directory(void)
// {
//     guarantee_stat_will_populate_stats_of_a_directory_type_file(CURRENT_DIRECTORY_PATH);
//     guarantee_readdir_will_fail();
//
//     scan_directory(CURRENT_DIRECTORY_PATH);
//
//     assert_file_entry_array_is_null();
// }

void register_scanner_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, test_teardown);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_return_NULL_if_a_NULL_path_is_specified", should_return_NULL_if_a_NULL_path_is_specified);
        CU_add_test(suite, "should_return_NULL_if_an_empty_path_is_specified", should_return_NULL_if_an_empty_path_is_specified);
        CU_add_test(suite, "should_return_one_entry_if_the_current_directory_has_one_file", should_return_one_entry_if_the_current_directory_has_one_file);
        CU_add_test(suite, "should_return_multiple_entries_if_the_current_directory_has_more_than_one_file", should_return_multiple_entries_if_the_current_directory_has_more_than_one_file);
        CU_add_test(suite, "should_return_an_array_of_entries_if_one_non_empty_directory_path_is_specified", should_return_an_array_of_entries_if_one_non_empty_directory_path_is_specified);
        CU_add_test(suite, "should_return_an_array_of_entries_without_hidden_files_if_a_directory_with_hidden_files_is_specified", should_return_an_array_of_entries_without_hidden_files_if_a_directory_with_hidden_files_is_specified);
        CU_add_test(suite, "should_return_an_array_of_entries_if_a_hidden_directory_is_specified", should_return_an_array_of_entries_if_a_hidden_directory_is_specified);
        CU_add_test(suite, "should_return_an_empty_array_if_the_specified_directory_is_empty", should_return_an_empty_array_if_the_specified_directory_is_empty);
        CU_add_test(suite, "should_return_an_empty_array_if_the_specified_directory_only_contains_hidden_files", should_return_an_empty_array_if_the_specified_directory_only_contains_hidden_files);
        // CU_add_test(suite, "should_return_NULL_if_fails_to_open_a_directory", should_return_NULL_if_fails_to_open_a_directory);
        // CU_add_test(suite, "should_return_NULL_if_fails_to_read_a_directory", should_return_NULL_if_fails_to_read_a_directory);
    }
}
