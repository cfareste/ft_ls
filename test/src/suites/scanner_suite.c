#include <string.h>
#include "suites.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "mocks.h"
#include "scanner.h"
#include "file_entry.h"
#include "parsed_arguments.h"

#define SUITE_NAME "scanner"
#define CURRENT_DIRECTORY_PATH "."

static t_result *result;
static t_file_entry_array *sut;
static t_scanner_context *scanner_context;

static void test_setup(void)
{
    reset_printing_buffer();
    vfs_mock_reset();
    scanner_context = scanner_context_create(NULL);
}

static void test_teardown(void)
{
    result_destroy(&result);
    file_entry_array_destroy(&sut);
    scanner_context_destroy(&scanner_context);
}

static void scan_directory(const char *path)
{
    result = scan(scanner_context, path);
    sut = result_get_value(result);
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

static void assert_file_entry_file_type_is(const t_file_entry *file_entry, const t_file_type file_type)
{
    CU_ASSERT_EQUAL(file_entry_get_file_type(file_entry), file_type);
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

static void assert_file_entry_array_types_are(const t_file_type *file_types)
{
    unsigned int i = 0;
    const unsigned int count = file_entry_array_get_length(sut);

    while (i < count)
    {
        const t_file_entry *file_entry = file_entry_array_get_at(sut, i);
        assert_file_entry_file_type_is(file_entry, file_types[i]);
        i++;
    }

    CU_ASSERT_PTR_EQUAL(file_types[i], FILE_TYPE_NONE);
}

static void should_return_NULL_if_a_NULL_path_is_specified(void)
{
    scan_directory(NULL);

    CU_ASSERT(verify_that_no_error_was_printed());
    CU_ASSERT_EQUAL(result_has_failed(result), 1);
    CU_ASSERT_PTR_NULL(result_get_error_context(result));
    assert_file_entry_array_is_null();
}

static void should_return_NULL_if_an_empty_path_is_specified(void)
{
    scan_directory("");

    CU_ASSERT(verify_that_no_error_was_printed());
    CU_ASSERT_EQUAL(result_has_failed(result), 1);
    CU_ASSERT_PTR_NULL(result_get_error_context(result));
    assert_file_entry_array_is_null();
}

static void should_return_one_entry_if_the_current_directory_has_one_file(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(CURRENT_DIRECTORY_PATH, ".", "..", "file"),
        MOCK_FILE("file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    scan_directory(CURRENT_DIRECTORY_PATH);
    const t_file_entry *file_entry = file_entry_array_get_at(sut, 0);

    CU_ASSERT(verify_that_no_error_was_printed());
    CU_ASSERT_EQUAL(result_has_failed(result), 0);
    assert_file_entry_array_length_is(1);
    assert_file_entry_name_is(file_entry, "file");
    assert_file_entry_file_type_is(file_entry, FILE_TYPE_REGULAR);
}

static void should_return_multiple_entries_if_the_current_directory_has_more_than_one_file(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(CURRENT_DIRECTORY_PATH, ".", "..", "multiple", "multiple2", "multiple3"),
        MOCK_FILE("multiple"),
        MOCK_FILE("multiple2"),
        MOCK_FILE("multiple3"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *expected_files_names[] = { "multiple", "multiple2", "multiple3", NULL };
    const t_file_type expected_file_types[] = { FILE_TYPE_REGULAR, FILE_TYPE_REGULAR, FILE_TYPE_REGULAR, FILE_TYPE_NONE };

    scan_directory(CURRENT_DIRECTORY_PATH);

    CU_ASSERT(verify_that_no_error_was_printed());
    CU_ASSERT_EQUAL(result_has_failed(result), 0);
    assert_file_entry_array_length_is(3);
    assert_file_entry_array_names_are(expected_files_names);
    assert_file_entry_array_types_are(expected_file_types);
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
    const t_file_type expected_file_types[] = { FILE_TYPE_REGULAR, FILE_TYPE_DIRECTORY, FILE_TYPE_REGULAR, FILE_TYPE_DIRECTORY, FILE_TYPE_NONE };

    scan_directory("valid_dir");

    CU_ASSERT(verify_that_no_error_was_printed());
    CU_ASSERT_EQUAL(result_has_failed(result), 0);
    assert_file_entry_array_length_is(4);
    assert_file_entry_array_names_are(expected_files_names);
    assert_file_entry_array_types_are(expected_file_types);
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
    const t_file_type expected_file_types[] = { FILE_TYPE_DIRECTORY, FILE_TYPE_DIRECTORY, FILE_TYPE_REGULAR, FILE_TYPE_NONE };

    scan_directory("dir");

    CU_ASSERT(verify_that_no_error_was_printed());
    CU_ASSERT_EQUAL(result_has_failed(result), 0);
    assert_file_entry_array_length_is(3);
    assert_file_entry_array_names_are(expected_file_names);
    assert_file_entry_array_types_are(expected_file_types);
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
    const t_file_type expected_file_types[] = { FILE_TYPE_REGULAR, FILE_TYPE_DIRECTORY, FILE_TYPE_NONE };

    scan_directory(".dir");

    CU_ASSERT(verify_that_no_error_was_printed());
    CU_ASSERT_EQUAL(result_has_failed(result), 0);
    assert_file_entry_array_length_is(2);
    assert_file_entry_array_names_are(expected_file_names);
    assert_file_entry_array_types_are(expected_file_types);
}

static void should_return_an_empty_array_if_the_specified_directory_is_empty(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(CURRENT_DIRECTORY_PATH, ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    scan_directory(CURRENT_DIRECTORY_PATH);

    CU_ASSERT(verify_that_no_error_was_printed());
    CU_ASSERT_EQUAL(result_has_failed(result), 0);
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

    CU_ASSERT(verify_that_no_error_was_printed());
    CU_ASSERT_EQUAL(result_has_failed(result), 0);
    assert_file_entry_array_length_is(0);
}

static void should_return_NULL_if_fails_to_open_a_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR_OPEN_ERROR(EACCES, "no_perm_dir", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    scan_directory("no_perm_dir");

    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot open directory '%s': %s\n",
        "no_perm_dir", strerror(errno))
    );
    CU_ASSERT_EQUAL(result_has_failed(result), 1);
    CU_ASSERT_STRING_EQUAL(result_get_error_context(result), "no_perm_dir");
    assert_file_entry_array_is_null();
}

static void should_return_an_empty_array_if_fails_to_read_the_first_entry_of_a_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR_READ_ERROR("read_dir", 0, ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    scan_directory("read_dir");

    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: reading directory '%s': %s\n",
        "read_dir", strerror(errno))
    );
    CU_ASSERT_EQUAL(result_has_failed(result), 1);
    CU_ASSERT_STRING_EQUAL(result_get_error_context(result), "read_dir");
    assert_file_entry_array_length_is(0);
}

static void should_return_an_array_with_the_elements_that_didnt_fail_if_fails_to_read_a_middle_entry_of_a_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR_READ_ERROR("read_dir", 2, "valid", "file", "..", ".", "failed"),
        MOCK_FILE("read_dir/valid"),
        MOCK_FILE("read_dir/file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *expected_file_names[] = { "valid", "file", NULL };
    const t_file_type expected_file_types[] = { FILE_TYPE_REGULAR, FILE_TYPE_REGULAR, FILE_TYPE_NONE };

    scan_directory("read_dir");

    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: reading directory '%s': %s\n",
        "read_dir", strerror(errno))
    );
    CU_ASSERT_EQUAL(result_has_failed(result), 1);
    CU_ASSERT_STRING_EQUAL(result_get_error_context(result), "read_dir");
    assert_file_entry_array_length_is(2);
    assert_file_entry_array_names_are(expected_file_names);
    assert_file_entry_array_types_are(expected_file_types);
}

static void should_return_a_valid_array_even_if_it_fails_to_close_a_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR_CLOSE_ERROR("close_error", "valid", ".", "dir", "..", "entries"),
        MOCK_FILE("close_error/valid"),
        MOCK_FILE("close_error/dir"),
        MOCK_FILE("close_error/entries"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *expected_file_names[] = { "valid", "dir", "entries", NULL };
    const t_file_type expected_file_types[] = { FILE_TYPE_REGULAR, FILE_TYPE_REGULAR, FILE_TYPE_REGULAR, FILE_TYPE_NONE };

    scan_directory("close_error");

    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: closing directory '%s': %s\n",
        "close_error", strerror(errno))
    );
    CU_ASSERT_EQUAL(result_has_failed(result), 1);
    CU_ASSERT_STRING_EQUAL(result_get_error_context(result), "close_error");
    assert_file_entry_array_length_is(3);
    assert_file_entry_array_names_are(expected_file_names);
    assert_file_entry_array_types_are(expected_file_types);
}

static void should_return_a_valid_array_even_if_it_fails_to_access_an_entry(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("valid_dir", "valid", ".", "dir", "..", "nonValid", "entries"),
        MOCK_FILE("valid_dir/valid"),
        MOCK_FILE("valid_dir/dir"),
        MOCK_FILE("valid_dir/entries"),
        MOCK_FILE_ACCESS_ERROR(ENOTDIR, "valid_dir/nonValid"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *expected_file_names[] = { "valid", "dir", "nonValid", "entries", NULL };
    const t_file_type expected_file_types[] = { FILE_TYPE_REGULAR, FILE_TYPE_REGULAR, FILE_TYPE_UNKNOWN, FILE_TYPE_REGULAR, FILE_TYPE_NONE };

    scan_directory("valid_dir");

    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n",
        "valid_dir/nonValid", strerror(ENOTDIR))
    );
    CU_ASSERT_TRUE(result_has_failed(result));
    CU_ASSERT_STRING_EQUAL(result_get_error_context(result), "valid_dir/nonValid");
    assert_file_entry_array_length_is(4);
    assert_file_entry_array_names_are(expected_file_names);
    assert_file_entry_array_types_are(expected_file_types);
}

static void should_retrieve_advanced_metadata_only_when_long_format_is_active(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "..", "link"),
        MOCK_SYMLINK_WITH_STATS("dir/link", "target",
            MOCK_AUTHOR("alice"),
            MOCK_GROUP("developers")),
        MOCK_FILE("target"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *arguments[] = { "-l", NULL };
    t_result *parsed_arguments_result = parse_arguments(1, arguments);
    t_parsed_arguments *parsed_arguments = result_get_value(parsed_arguments_result);
    scanner_context_destroy(&scanner_context);
    scanner_context = scanner_context_create(parsed_arguments);

    scan_directory("dir");

    const t_file_entry *entry = file_entry_array_get_at(sut, 0);
    CU_ASSERT(verify_that_no_error_was_printed());
    CU_ASSERT_STRING_EQUAL(file_entry_get_author(entry), "alice");
    CU_ASSERT_STRING_EQUAL(file_entry_get_group(entry), "developers");
    CU_ASSERT_STRING_EQUAL(file_entry_get_link_target(entry), "target");

    parsed_arguments_destroy(&parsed_arguments);
    result_destroy(&parsed_arguments_result);
}

static void should_not_retrieve_advanced_metadata_without_long_format(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "..", "link"),
        MOCK_READ_SYM_LINK_ERROR(EINVAL, "dir/link", "target"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    scan_directory("dir");

    const t_file_entry *entry = file_entry_array_get_at(sut, 0);
    CU_ASSERT(verify_that_no_error_was_printed());
    CU_ASSERT_PTR_NULL(file_entry_get_author(entry));
    CU_ASSERT_PTR_NULL(file_entry_get_group(entry));
    CU_ASSERT_PTR_NULL(file_entry_get_link_target(entry));
}

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
        CU_add_test(suite, "should_return_NULL_if_fails_to_open_a_directory", should_return_NULL_if_fails_to_open_a_directory);
        CU_add_test(suite, "should_return_an_empty_array_if_fails_to_read_the_first_entry_of_a_directory", should_return_an_empty_array_if_fails_to_read_the_first_entry_of_a_directory);
        CU_add_test(suite, "should_return_an_array_with_the_elements_that_didnt_fail_if_fails_to_read_a_middle_entry_of_a_directory", should_return_an_array_with_the_elements_that_didnt_fail_if_fails_to_read_a_middle_entry_of_a_directory);
        CU_add_test(suite, "should_return_a_valid_array_even_if_it_fails_to_close_a_directory", should_return_a_valid_array_even_if_it_fails_to_close_a_directory);
        CU_add_test(suite, "should_return_a_valid_array_even_if_it_fails_to_access_an_entry", should_return_a_valid_array_even_if_it_fails_to_access_an_entry);
        CU_add_test(suite, "should_retrieve_advanced_metadata_only_when_long_format_is_active", should_retrieve_advanced_metadata_only_when_long_format_is_active);
        CU_add_test(suite, "should_not_retrieve_advanced_metadata_without_long_format", should_not_retrieve_advanced_metadata_without_long_format);
    }
}
