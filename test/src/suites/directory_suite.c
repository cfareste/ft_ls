#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "directory.h"
#include "mocks.h"

#define SUITE_NAME "directory"
#define VALID_DIRECTORY_PATH "valid"

static t_dir_stream *dir_stream_sut;

static void test_setup(void)
{
    vfs_mock_reset();
}

static void test_teardown(void)
{
    directory_close(&dir_stream_sut);
}

static void open_directory_stream(const char *path)
{
    dir_stream_sut = directory_open(path);
}

static void assert_dir_stream_is_null(void)
{
    CU_ASSERT_PTR_NULL(dir_stream_sut);
}

static void should_return_NULL_when_opening_a_NULL_path(void)
{
    open_directory_stream(NULL);

    assert_dir_stream_is_null();
}

static void should_return_NULL_when_opening_an_empty_path(void)
{
    open_directory_stream("");

    assert_dir_stream_is_null();
}

static void should_return_a_directory_stream_when_opening_a_valid_path(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(VALID_DIRECTORY_PATH, ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    open_directory_stream(VALID_DIRECTORY_PATH);

    CU_ASSERT_PTR_NOT_NULL(dir_stream_sut);
}

static void should_return_NULL_for_next_entry_if_a_NULL_dir_stream_is_specified(void)
{
    const t_dir_entry *dir_entry = directory_get_next(NULL);

    CU_ASSERT_PTR_NULL(dir_entry);
}

static void should_return_the_next_entry_when_reading_from_a_valid_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", "file", ".", ".."),
        MOCK_FILE("./file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    open_directory_stream(".");

    t_dir_entry *dir_entry = directory_get_next(dir_stream_sut);

    CU_ASSERT_STRING_EQUAL(directory_get_entry_name(dir_entry), "file");

    directory_entry_destroy(&dir_entry);
}

static void should_not_fail_to_destroy_a_null_pointer_dir_entry(void)
{
    directory_entry_destroy(NULL);
}

static void should_not_fail_to_destroy_a_null_dir_entry(void)
{
    t_dir_entry *dir_entry = NULL;

    directory_entry_destroy(&dir_entry);
}

static void should_destroy_a_valid_dir_entry_correctly(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", "file", ".", ".."),
        MOCK_FILE("./file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    open_directory_stream(".");
    t_dir_entry *dir_entry = directory_get_next(dir_stream_sut);

    directory_entry_destroy(&dir_entry);

    CU_ASSERT_PTR_NULL(dir_entry);
}

static void should_return_NULL_for_the_entry_name_if_a_NULL_entry_is_specified(void)
{
    const char *invalid_name = directory_get_entry_name(NULL);

    CU_ASSERT_PTR_NULL(invalid_name);
}

static void should_return_the_dir_entry_name(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", "valid file", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    open_directory_stream(".");
    t_dir_entry *dir_entry = directory_get_next(dir_stream_sut);

    CU_ASSERT_STRING_EQUAL(directory_get_entry_name(dir_entry), "valid file");

    directory_entry_destroy(&dir_entry);
}

static void should_return_NULL_when_reading_an_entry_from_a_null_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(VALID_DIRECTORY_PATH, ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    open_directory_stream(VALID_DIRECTORY_PATH);

    const struct dirent *entry = directory_get_next_entry(NULL);

    CU_ASSERT_PTR_NULL(entry);
}

static void should_return_an_entry_when_reading_from_a_valid_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(VALID_DIRECTORY_PATH, "file", ".", ".."),
        MOCK_FILE(VALID_DIRECTORY_PATH "/file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *file_name = "file";
    open_directory_stream(VALID_DIRECTORY_PATH);

    const struct dirent *entry = directory_get_next_entry(dir_stream_sut);

    CU_ASSERT_PTR_NOT_NULL(entry);
    CU_ASSERT_STRING_EQUAL(entry->d_name, file_name);
}

static void should_return_minus_one_when_closing_a_null_pointer(void)
{
    const int actual = directory_close(NULL);

    CU_ASSERT_EQUAL(actual, -1);
}

static void should_return_minus_one_when_closing_a_null_directory(void)
{
    t_dir_stream *dir_stream = NULL;

    CU_ASSERT_EQUAL(directory_close(&dir_stream), -1);
}

static void should_return_zero_when_closing_a_valid_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(VALID_DIRECTORY_PATH, ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    open_directory_stream(VALID_DIRECTORY_PATH);

    const int actual = directory_close(&dir_stream_sut);

    assert_dir_stream_is_null();
    CU_ASSERT_EQUAL(actual, 0);
}

void register_directory_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, test_teardown);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_return_NULL_when_opening_a_NULL_path", should_return_NULL_when_opening_a_NULL_path);
        CU_add_test(suite, "should_return_NULL_when_opening_an_empty_path", should_return_NULL_when_opening_an_empty_path);
        CU_add_test(suite, "should_return_a_directory_stream_when_opening_a_valid_path", should_return_a_directory_stream_when_opening_a_valid_path);
        CU_add_test(suite, "should_return_NULL_for_next_entry_if_a_NULL_dir_stream_is_specified", should_return_NULL_for_next_entry_if_a_NULL_dir_stream_is_specified);
        CU_add_test(suite, "should_return_the_next_entry_when_reading_from_a_valid_directory", should_return_the_next_entry_when_reading_from_a_valid_directory);
        CU_add_test(suite, "should_not_fail_to_destroy_a_null_pointer_dir_entry", should_not_fail_to_destroy_a_null_pointer_dir_entry);
        CU_add_test(suite, "should_not_fail_to_destroy_a_null_dir_entry", should_not_fail_to_destroy_a_null_dir_entry);
        CU_add_test(suite, "should_destroy_a_valid_dir_entry_correctly", should_destroy_a_valid_dir_entry_correctly);
        CU_add_test(suite, "should_return_NULL_for_the_entry_name_if_a_NULL_entry_is_specified", should_return_NULL_for_the_entry_name_if_a_NULL_entry_is_specified);
        CU_add_test(suite, "should_return_the_dir_entry_name", should_return_the_dir_entry_name);
        CU_add_test(suite, "should_return_NULL_when_reading_an_entry_from_a_null_directory", should_return_NULL_when_reading_an_entry_from_a_null_directory);
        CU_add_test(suite, "should_return_an_entry_when_reading_from_a_valid_directory", should_return_an_entry_when_reading_from_a_valid_directory);
        CU_add_test(suite, "should_return_minus_one_when_closing_a_null_pointer", should_return_minus_one_when_closing_a_null_pointer);
        CU_add_test(suite, "should_return_minus_one_when_closing_a_null_directory", should_return_minus_one_when_closing_a_null_directory);
        CU_add_test(suite, "should_return_zero_when_closing_a_valid_directory", should_return_zero_when_closing_a_valid_directory);
    }
}
