#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "mocks.h"
#include "file_stats.h"

#define SUITE_NAME "file_stats"

static void test_setup(void)
{
    vfs_mock_reset();
}

static void should_create_file_stats_correctly(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("valid_file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    t_file_stats *stats = file_stats_get("valid_file");

    CU_ASSERT_PTR_NOT_NULL(stats);

    file_stats_destroy(&stats);
}

static void should_return_NULL_when_creating_file_stats_with_a_NULL_path(void)
{
    const t_file_stats *stats = file_stats_get(NULL);

    CU_ASSERT_PTR_NULL(stats);
}

static void should_return_NULL_when_creating_file_stats_with_an_empty_path(void)
{
    const t_file_stats *stats = file_stats_get("");

    CU_ASSERT_PTR_NULL(stats);
}

static void should_destroy_file_stats_correctly(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("valid_file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    t_file_stats *stats = file_stats_get("valid_file");

    file_stats_destroy(&stats);

    CU_ASSERT_PTR_NULL(stats);
}

static void should_not_fail_to_destroy_file_stats_if_a_NULL_pointer_is_passed(void)
{
    file_stats_destroy(NULL);
}

static void should_not_fail_to_destroy_file_stats_if_a_NULL_file_stats_is_passed(void)
{
    t_file_stats *invalid_stats = NULL;

    file_stats_destroy(&invalid_stats);
}

static void should_return_unknown_file_type_when_a_NULL_file_stats_are_specified(void)
{
    const t_file_type invalid = file_stats_get_file_type(NULL);

    CU_ASSERT_EQUAL(invalid, FILE_TYPE_UNKNOWN);
}

static void should_return_the_file_type_of_the_specified_file_stats(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("reg_file"),
        MOCK_DIR("dir", ".", ".."),
        MOCK_CHAR_DEVICE("char_device"),
        MOCK_BLOCK_DEVICE("block_device"),
        MOCK_FIFO("pipe"),
        MOCK_SYMLINK("symlink", "dir"),
        MOCK_SOCKET("socket"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    t_file_stats *reg_file_stats = file_stats_get("reg_file");
    t_file_stats *dir_stats = file_stats_get("dir");
    t_file_stats *chardevice_stats = file_stats_get("char_device");
    t_file_stats *blockdevice_stats = file_stats_get("block_device");
    t_file_stats *fifo_stats = file_stats_get("pipe");
    t_file_stats *symlink_stats = file_stats_get("symlink");
    t_file_stats *socket_stats = file_stats_get("socket");

    CU_ASSERT_EQUAL(file_stats_get_file_type(reg_file_stats), FILE_TYPE_REGULAR);
    CU_ASSERT_EQUAL(file_stats_get_file_type(dir_stats), FILE_TYPE_DIRECTORY);
    CU_ASSERT_EQUAL(file_stats_get_file_type(chardevice_stats), FILE_TYPE_CHARDEVICE);
    CU_ASSERT_EQUAL(file_stats_get_file_type(blockdevice_stats), FILE_TYPE_BLOCKDEVICE);
    CU_ASSERT_EQUAL(file_stats_get_file_type(fifo_stats), FILE_TYPE_FIFO);
    CU_ASSERT_EQUAL(file_stats_get_file_type(symlink_stats), FILE_TYPE_DIRECTORY);
    CU_ASSERT_EQUAL(file_stats_get_file_type(socket_stats), FILE_TYPE_SOCKET);

    file_stats_destroy(&reg_file_stats);
    file_stats_destroy(&dir_stats);
    file_stats_destroy(&chardevice_stats);
    file_stats_destroy(&blockdevice_stats);
    file_stats_destroy(&fifo_stats);
    file_stats_destroy(&symlink_stats);
    file_stats_destroy(&socket_stats);
}

void register_file_stats_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_create_file_stats_correctly", should_create_file_stats_correctly);
        CU_add_test(suite, "should_return_NULL_when_creating_file_stats_with_an_empty_path", should_return_NULL_when_creating_file_stats_with_an_empty_path);
        CU_add_test(suite, "should_return_NULL_when_creating_file_stats_with_a_NULL_path", should_return_NULL_when_creating_file_stats_with_a_NULL_path);
        CU_add_test(suite, "should_destroy_file_stats_correctly", should_destroy_file_stats_correctly);
        CU_add_test(suite, "should_not_fail_to_destroy_file_stats_if_a_NULL_pointer_is_passed", should_not_fail_to_destroy_file_stats_if_a_NULL_pointer_is_passed);
        CU_add_test(suite, "should_not_fail_to_destroy_file_stats_if_a_NULL_file_stats_is_passed", should_not_fail_to_destroy_file_stats_if_a_NULL_file_stats_is_passed);
        CU_add_test(suite, "should_return_unknown_file_type_when_a_NULL_file_stats_are_specified", should_return_unknown_file_type_when_a_NULL_file_stats_are_specified);
        CU_add_test(suite, "should_return_the_file_type_of_the_specified_file_stats", should_return_the_file_type_of_the_specified_file_stats);
    }
}
