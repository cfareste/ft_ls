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
    const t_dir_entry *dir_entry = directory_get_next_entry(NULL);

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

    t_dir_entry *dir_entry = directory_get_next_entry(dir_stream_sut);

    CU_ASSERT_STRING_EQUAL(directory_get_entry_name(dir_entry), "file");

    directory_destroy_entry(&dir_entry);
}

static void should_not_fail_to_destroy_a_null_pointer_dir_entry(void)
{
    directory_destroy_entry(NULL);
}

static void should_not_fail_to_destroy_a_null_dir_entry(void)
{
    t_dir_entry *dir_entry = NULL;

    directory_destroy_entry(&dir_entry);
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
    t_dir_entry *dir_entry = directory_get_next_entry(dir_stream_sut);

    directory_destroy_entry(&dir_entry);

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
    t_dir_entry *dir_entry = directory_get_next_entry(dir_stream_sut);

    CU_ASSERT_STRING_EQUAL(directory_get_entry_name(dir_entry), "valid file");

    directory_destroy_entry(&dir_entry);
}

static void should_return_true_for_is_entry_empty_if_the_dir_entry_is_NULL(void)
{
    CU_ASSERT_TRUE(directory_is_entry_empty(NULL));
}

static void should_return_false_for_is_entry_empty_if_the_dir_entry_is_not_empty(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", "file", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    open_directory_stream(".");
    t_dir_entry *first_entry = directory_get_next_entry(dir_stream_sut);

    CU_ASSERT_FALSE(directory_is_entry_empty(first_entry));

    directory_destroy_entry(&first_entry);
}

static void should_return_true_for_is_entry_empty_if_the_dir_entry_is_empty(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    open_directory_stream(".");
    t_dir_entry *first_entry = directory_get_next_entry(dir_stream_sut);
    directory_destroy_entry(&first_entry);

    t_dir_entry *second_entry = directory_get_next_entry(dir_stream_sut);

    CU_ASSERT_TRUE(directory_is_entry_empty(second_entry));

    directory_destroy_entry(&second_entry);
}

static void should_return_false_for_is_hidden_file_entry_if_the_dir_entry_is_NULL(void)
{
    CU_ASSERT_FALSE(directory_is_entry_hidden_file(NULL));
}

static void should_return_false_for_is_hidden_file_entry_if_the_entry_points_to_a_non_hidden_file(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", "reg_file", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    open_directory_stream(".");
    t_dir_entry *first_entry = directory_get_next_entry(dir_stream_sut);

    CU_ASSERT_FALSE(directory_is_entry_hidden_file(first_entry));

    directory_destroy_entry(&first_entry);
}

static void should_return_true_for_is_hidden_file_entry_if_the_entry_points_to_a_hidden_file(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".hidden_file", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    open_directory_stream(".");
    t_dir_entry *first_entry = directory_get_next_entry(dir_stream_sut);

    CU_ASSERT_TRUE(directory_is_entry_hidden_file(first_entry));

    directory_destroy_entry(&first_entry);
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

    const int result = directory_close(&dir_stream_sut);

    assert_dir_stream_is_null();
    CU_ASSERT_EQUAL(result, 0);
}

static void should_return_NULL_when_opening_a_directory_without_permissions(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR_OPEN_ERROR(EACCES, "noPerm", ".", ".."),
        MOCK_DIR_OPEN_ERROR(EMFILE, "processFD", ".", ".."),
        MOCK_DIR_OPEN_ERROR(ENAMETOOLONG, "nameTooLong", ".", ".."),
        MOCK_DIR_OPEN_ERROR(ENFILE, "systemFD", ".", ".."),
        MOCK_DIR_OPEN_ERROR(ENOTDIR, "notADirectory", ".", ".."),
        MOCK_DIR_OPEN_ERROR(ENOMEM, "noMemory", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    open_directory_stream("noPerm");
    open_directory_stream("processFD");
    open_directory_stream("nameTooLong");
    open_directory_stream("systemFD");
    open_directory_stream("dirDoesntExist");
    open_directory_stream("notADirectory");
    open_directory_stream("noMemory");

    CU_ASSERT(verify_that_the_error_that_has_been_printed_is(
        "ft_ls: cannot open directory '%s': Permission denied\n"
        "ft_ls: cannot open directory '%s': Too many open files\n"
        "ft_ls: cannot open directory '%s': File name too long\n"
        "ft_ls: cannot open directory '%s': Too many open files in system\n"
        "ft_ls: cannot open directory '%s': No such file or directory\n"
        "ft_ls: cannot open directory '%s': Not a directory\n"
        "ft_ls: cannot open directory '%s': Cannot allocate memory\n",
        "noPerm", "processFD", "nameTooLong", "systemFD", "dirDoesntExist", "notADirectory", "noMemory"));
    assert_dir_stream_is_null();
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
        CU_add_test(suite, "should_return_true_for_is_entry_empty_if_the_dir_entry_is_NULL", should_return_true_for_is_entry_empty_if_the_dir_entry_is_NULL);
        CU_add_test(suite, "should_return_false_for_is_entry_empty_if_the_dir_entry_is_not_empty", should_return_false_for_is_entry_empty_if_the_dir_entry_is_not_empty);
        CU_add_test(suite, "should_return_true_for_is_entry_empty_if_the_dir_entry_is_empty", should_return_true_for_is_entry_empty_if_the_dir_entry_is_empty);
        CU_add_test(suite, "should_return_false_for_is_hidden_file_entry_if_the_dir_entry_is_NULL", should_return_false_for_is_hidden_file_entry_if_the_dir_entry_is_NULL);
        CU_add_test(suite, "should_return_false_for_is_hidden_file_entry_if_the_entry_points_to_a_non_hidden_file", should_return_false_for_is_hidden_file_entry_if_the_entry_points_to_a_non_hidden_file);
        CU_add_test(suite, "should_return_true_for_is_hidden_file_entry_if_the_entry_points_to_a_hidden_file", should_return_true_for_is_hidden_file_entry_if_the_entry_points_to_a_hidden_file);
        CU_add_test(suite, "should_return_minus_one_when_closing_a_null_pointer", should_return_minus_one_when_closing_a_null_pointer);
        CU_add_test(suite, "should_return_minus_one_when_closing_a_null_directory", should_return_minus_one_when_closing_a_null_directory);
        CU_add_test(suite, "should_return_zero_when_closing_a_valid_directory", should_return_zero_when_closing_a_valid_directory);
        CU_add_test(suite, "should_return_NULL_when_opening_a_directory_without_permissions", should_return_NULL_when_opening_a_directory_without_permissions);
    }
}
