#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "mocks.h"
#include "file_stats.h"

#define SUITE_NAME "file_stats"

static void test_setup(void)
{
    reset_printing_buffer();
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
    CU_ASSERT(verify_that_no_error_was_printed());

    file_stats_destroy(&stats);
}

static void should_return_NULL_when_creating_file_stats_with_a_NULL_path(void)
{
    const t_file_stats *stats = file_stats_get(NULL);

    CU_ASSERT_PTR_NULL(stats);
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_NULL_when_creating_file_stats_with_an_empty_path(void)
{
    const t_file_stats *stats = file_stats_get("");

    CU_ASSERT_PTR_NULL(stats);
    CU_ASSERT(verify_that_no_error_was_printed());
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
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_not_fail_to_destroy_file_stats_if_a_NULL_pointer_is_passed(void)
{
    file_stats_destroy(NULL);

    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_not_fail_to_destroy_file_stats_if_a_NULL_file_stats_is_passed(void)
{
    t_file_stats *invalid_stats = NULL;

    file_stats_destroy(&invalid_stats);

    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_unknown_file_type_when_a_NULL_file_stats_are_specified(void)
{
    const t_file_type invalid = file_stats_get_file_type(NULL);

    CU_ASSERT_EQUAL(invalid, FILE_TYPE_UNKNOWN);
    CU_ASSERT(verify_that_no_error_was_printed());
}

static void should_return_the_file_stats_of_the_specified_non_link_files(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("reg_file"),
        MOCK_DIR("dir", ".", ".."),
        MOCK_CHAR_DEVICE("char_device"),
        MOCK_BLOCK_DEVICE("block_device"),
        MOCK_FIFO("pipe"),
        MOCK_SOCKET("socket"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    t_file_stats *reg_file_stats = file_stats_get("reg_file");
    t_file_stats *dir_stats = file_stats_get("dir");
    t_file_stats *chardevice_stats = file_stats_get("char_device");
    t_file_stats *blockdevice_stats = file_stats_get("block_device");
    t_file_stats *fifo_stats = file_stats_get("pipe");
    t_file_stats *socket_stats = file_stats_get("socket");

    CU_ASSERT_EQUAL(file_stats_get_file_type(reg_file_stats), FILE_TYPE_REGULAR);
    CU_ASSERT_EQUAL(file_stats_get_file_type(dir_stats), FILE_TYPE_DIRECTORY);
    CU_ASSERT_EQUAL(file_stats_get_file_type(chardevice_stats), FILE_TYPE_CHARDEVICE);
    CU_ASSERT_EQUAL(file_stats_get_file_type(blockdevice_stats), FILE_TYPE_BLOCKDEVICE);
    CU_ASSERT_EQUAL(file_stats_get_file_type(fifo_stats), FILE_TYPE_FIFO);
    CU_ASSERT_EQUAL(file_stats_get_file_type(socket_stats), FILE_TYPE_SOCKET);
    CU_ASSERT(verify_that_no_error_was_printed());

    file_stats_destroy(&reg_file_stats);
    file_stats_destroy(&dir_stats);
    file_stats_destroy(&chardevice_stats);
    file_stats_destroy(&blockdevice_stats);
    file_stats_destroy(&fifo_stats);
    file_stats_destroy(&socket_stats);
}

static void should_return_link_file_stats_if_the_specified_files_are_symlinks_to_non_directories(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("reg"),
        MOCK_CHAR_DEVICE("char_device"),
        MOCK_BLOCK_DEVICE("block_device"),
        MOCK_FIFO("pipe"),
        MOCK_SOCKET("socket"),
        MOCK_SYMLINK("sym_reg", "reg"),
        MOCK_SYMLINK("sym_char", "char_device"),
        MOCK_SYMLINK("sym_block", "block_device"),
        MOCK_SYMLINK("sym_pipe", "pipe"),
        MOCK_SYMLINK("sym_sock", "socket"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    t_file_stats *reg_link_stats = file_stats_get("sym_reg");
    t_file_stats *char_link_stats = file_stats_get("sym_char");
    t_file_stats *block_link_stats = file_stats_get("sym_block");
    t_file_stats *pipe_link_stats = file_stats_get("sym_pipe");
    t_file_stats *sock_link_stats = file_stats_get("sym_sock");

    CU_ASSERT_EQUAL(file_stats_get_file_type(reg_link_stats), FILE_TYPE_SYMLINK);
    CU_ASSERT_EQUAL(file_stats_get_file_type(char_link_stats), FILE_TYPE_SYMLINK);
    CU_ASSERT_EQUAL(file_stats_get_file_type(block_link_stats), FILE_TYPE_SYMLINK);
    CU_ASSERT_EQUAL(file_stats_get_file_type(pipe_link_stats), FILE_TYPE_SYMLINK);
    CU_ASSERT_EQUAL(file_stats_get_file_type(sock_link_stats), FILE_TYPE_SYMLINK);
    CU_ASSERT(verify_that_no_error_was_printed());

    file_stats_destroy(&reg_link_stats);
    file_stats_destroy(&char_link_stats);
    file_stats_destroy(&block_link_stats);
    file_stats_destroy(&pipe_link_stats);
    file_stats_destroy(&sock_link_stats);
}

static void should_return_directory_file_stats_if_the_specified_files_are_symlinks_to_directories(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", ".."),
        MOCK_SYMLINK("sym_dir", "dir"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    t_file_stats *dir_link_stats = file_stats_get("sym_dir");

    CU_ASSERT_EQUAL(file_stats_get_file_type(dir_link_stats), FILE_TYPE_DIRECTORY);
    CU_ASSERT(verify_that_no_error_was_printed());

    file_stats_destroy(&dir_link_stats);
}

static void should_return_link_file_stats_if_the_specified_file_is_a_broken_symlink(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_BROKEN_LINK("brokenLink"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    t_file_stats *broken_link_stats = file_stats_get("brokenLink");

    CU_ASSERT_EQUAL(file_stats_get_file_type(broken_link_stats), FILE_TYPE_SYMLINK);
    CU_ASSERT(verify_that_no_error_was_printed());

    file_stats_destroy(&broken_link_stats);
}

static void should_return_NULL_when_an_error_accessing_a_file_occurs(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_ACCESS_ERROR(EACCES, "noPermissions"),
        MOCK_FILE_ACCESS_ERROR(EFAULT, "badAddress"),
        MOCK_FILE_ACCESS_ERROR(ELOOP, "symlinkLoop"),
        MOCK_FILE_ACCESS_ERROR(ENAMETOOLONG, "nameTooLong"),
        MOCK_FILE_ACCESS_ERROR(ENOENT, "nonExisting"),
        MOCK_FILE_ACCESS_ERROR(ENOMEM, "noMemory"),
        MOCK_FILE_ACCESS_ERROR(ENOTDIR, "notADirectory"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    CU_ASSERT_PTR_NULL(file_stats_get("noPermissions"));
    CU_ASSERT_PTR_NULL(file_stats_get("badAddress"));
    CU_ASSERT_PTR_NULL(file_stats_get("symlinkLoop"));
    CU_ASSERT_PTR_NULL(file_stats_get("nameTooLong"));
    CU_ASSERT_PTR_NULL(file_stats_get("nonExisting"));
    CU_ASSERT_PTR_NULL(file_stats_get("noMemory"));
    CU_ASSERT_PTR_NULL(file_stats_get("notADirectory"));
    CU_ASSERT(verify_that_the_error_printed_is(
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot access '%s': %s\n"
        "ft_ls: cannot access '%s': %s\n",
        "noPermissions", strerror(EACCES),
        "badAddress", strerror(EFAULT),
        "symlinkLoop", strerror(ELOOP),
        "nameTooLong", strerror(ENAMETOOLONG),
        "nonExisting", strerror(ENOENT),
        "noMemory", strerror(ENOMEM),
        "notADirectory", strerror(ENOTDIR)
    ));
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
        CU_add_test(suite, "should_return_the_file_stats_of_the_specified_non_link_files", should_return_the_file_stats_of_the_specified_non_link_files);
        CU_add_test(suite, "should_return_link_file_stats_if_the_specified_files_are_symlinks_to_non_directories", should_return_link_file_stats_if_the_specified_files_are_symlinks_to_non_directories);
        CU_add_test(suite, "should_return_directory_file_stats_if_the_specified_files_are_symlinks_to_directories", should_return_directory_file_stats_if_the_specified_files_are_symlinks_to_directories);
        CU_add_test(suite, "should_return_link_file_stats_if_the_specified_file_is_a_broken_symlink", should_return_link_file_stats_if_the_specified_file_is_a_broken_symlink);
        CU_add_test(suite, "should_return_NULL_when_an_error_accessing_a_file_occurs", should_return_NULL_when_an_error_accessing_a_file_occurs);
    }
}
