#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "directory.h"
#include "mocks.h"

#define SUITE_NAME "directory"
#define VALID_DIRECTORY_PATH "valid"

static DIR *dir_stream_sut;

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
        MOCK_DIR(VALID_DIRECTORY_PATH, ".", "..")
    };
    vfs_mock_setup(vfs);

    open_directory_stream(VALID_DIRECTORY_PATH);

    CU_ASSERT_PTR_NOT_NULL(dir_stream_sut);
}

static void should_return_NULL_when_reading_an_entry_from_a_null_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(VALID_DIRECTORY_PATH, ".", "..")
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
        MOCK_FILE(VALID_DIRECTORY_PATH "/file")
    };
    vfs_mock_setup(vfs);

    const char *file_name = "file";
    open_directory_stream(VALID_DIRECTORY_PATH);

    const struct dirent *entry = directory_get_next_entry(dir_stream_sut);

    CU_ASSERT_PTR_NOT_NULL(entry);
    CU_ASSERT_STRING_EQUAL(entry->d_name, file_name);
}

static void should_return_minus_one_when_closing_a_null_directory(void)
{
    const int actual = directory_close(NULL);

    CU_ASSERT_EQUAL(actual, -1);
}

static void should_return_zero_when_closing_a_valid_directory(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(VALID_DIRECTORY_PATH, ".", "..")
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
        CU_add_test(suite, "should_return_NULL_when_reading_an_entry_from_a_null_directory", should_return_NULL_when_reading_an_entry_from_a_null_directory);
        CU_add_test(suite, "should_return_an_entry_when_reading_from_a_valid_directory", should_return_an_entry_when_reading_from_a_valid_directory);
        CU_add_test(suite, "should_return_minus_one_when_closing_a_null_directory", should_return_minus_one_when_closing_a_null_directory);
        CU_add_test(suite, "should_return_zero_when_closing_a_valid_directory", should_return_zero_when_closing_a_valid_directory);
    }
}
