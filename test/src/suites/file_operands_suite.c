#include <stdlib.h>
#include "suites.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "file_operands.h"
#include "mocks.h"

#define SUITE_NAME "file_operands"

static void test_setup(void)
{
    vfs_mock_reset();
    reset_printing_buffer();
}

static void test_teardown(void)
{
    vfs_mock_reset();
}

static void destroy_string_array(char **array)
{
    if (array == NULL)
        return ;

    for (size_t i = 0; array[i] != NULL; i++)
        free(array[i]);
    free(array);
}

static void destroy_stats_array(t_file_stats **stats, const int count)
{
    if (stats == NULL)
        return ;

    for (int i = 0; i < count; i++)
        file_stats_destroy(&stats[i]);
    free(stats);
}

static void should_return_the_default_directory_for_zero_operands(void)
{
    char **operands = file_operands_get(0, NULL);

    CU_ASSERT_PTR_NOT_NULL(operands);
    CU_ASSERT_STRING_EQUAL(operands[0], ".");
    CU_ASSERT_PTR_NULL(operands[1]);

    destroy_string_array(operands);
}

static void should_copy_operands_in_order_and_skip_option_arguments(void)
{
    const char *arguments[] = { "-la", "", "directory", "-r", NULL };
    char **operands = file_operands_get(2, arguments);

    CU_ASSERT_PTR_NOT_NULL(operands);
    CU_ASSERT_STRING_EQUAL(operands[0], "");
    CU_ASSERT_STRING_EQUAL(operands[1], "directory");
    CU_ASSERT_PTR_NULL(operands[2]);

    destroy_string_array(operands);
}

static void should_reject_missing_or_negative_operand_counts(void)
{
    const char *arguments[] = { "file", NULL };

    CU_ASSERT_PTR_NULL(file_operands_get(1, NULL));
    CU_ASSERT_PTR_NULL(file_operands_get(-1, arguments));
}

static void should_retrieve_stats_with_or_without_following_symlinks(void)
{
    const t_vfs_mock_entry entries[] = {
        MOCK_SYMLINK("link", "target"),
        MOCK_DIR("target", "child"),
        MOCK_NULL_TERMINATOR()
    };
    char *operands[] = { "link", NULL };
    t_file_stats **followed_stats;
    t_file_stats **unfollowed_stats;

    vfs_mock_setup(entries);
    followed_stats = file_operands_get_stats(1, operands, 1);
    unfollowed_stats = file_operands_get_stats(1, operands, 0);

    CU_ASSERT_PTR_NOT_NULL(followed_stats);
    CU_ASSERT_PTR_NOT_NULL(unfollowed_stats);
    CU_ASSERT_EQUAL(file_stats_get_file_type(followed_stats[0]), FILE_TYPE_DIRECTORY);
    CU_ASSERT_EQUAL(file_stats_get_file_type(unfollowed_stats[0]), FILE_TYPE_SYMLINK);
    CU_ASSERT_PTR_NULL(followed_stats[1]);
    CU_ASSERT_PTR_NULL(unfollowed_stats[1]);

    destroy_stats_array(followed_stats, 1);
    destroy_stats_array(unfollowed_stats, 1);
}

static void should_return_an_empty_stats_array_for_zero_operands(void)
{
    t_file_stats **stats = file_operands_get_stats(0, NULL, 1);

    CU_ASSERT_PTR_NOT_NULL(stats);
    CU_ASSERT_PTR_NULL(stats[0]);

    destroy_stats_array(stats, 0);
}

static void should_reject_missing_or_negative_stats_inputs(void)
{
    char *operands[] = { "file", NULL };

    CU_ASSERT_PTR_NULL(file_operands_get_stats(1, NULL, 1));
    CU_ASSERT_PTR_NULL(file_operands_get_stats(-1, operands, 1));
}

static void should_select_non_directory_operands_and_preserve_order(void)
{
    const t_vfs_mock_entry entries[] = {
        MOCK_FILE("file"),
        MOCK_DIR("directory", "child"),
        MOCK_SYMLINK("link", "file"),
        MOCK_NULL_TERMINATOR()
    };
    char *operands[] = { "file", "directory", "link", NULL };
    t_file_stats **stats;
    char **non_directories;

    vfs_mock_setup(entries);
    stats = file_operands_get_stats(3, operands, 0);
    non_directories = file_operands_get_non_directory(operands, stats, 0);

    CU_ASSERT_PTR_NOT_NULL(non_directories);
    CU_ASSERT_STRING_EQUAL(non_directories[0], "file");
    CU_ASSERT_STRING_EQUAL(non_directories[1], "link");
    CU_ASSERT_PTR_NULL(non_directories[2]);

    destroy_string_array(non_directories);
    destroy_stats_array(stats, 3);
}

static void should_select_directories_including_an_implicit_default_operand(void)
{
    const t_vfs_mock_entry entries[] = {
        MOCK_DIR(".", "child"),
        MOCK_FILE("file"),
        MOCK_NULL_TERMINATOR()
    };
    char *operands[] = { ".", "file", NULL };
    t_file_stats **stats;
    char **directories;

    vfs_mock_setup(entries);
    stats = file_operands_get_stats(2, operands, 1);
    directories = file_operands_get_directory(operands, stats, 1);

    CU_ASSERT_PTR_NOT_NULL(directories);
    CU_ASSERT_STRING_EQUAL(directories[0], ".");
    CU_ASSERT_PTR_NULL(directories[1]);

    destroy_string_array(directories);
    destroy_stats_array(stats, 2);
}

static void should_reject_null_operand_or_stats_arrays_when_selecting_categories(void)
{
    char *operands[] = { NULL };
    t_file_stats *stats[] = { NULL };

    CU_ASSERT_PTR_NULL(file_operands_get_non_directory(NULL, stats, 0));
    CU_ASSERT_PTR_NULL(file_operands_get_non_directory(operands, NULL, 0));
    CU_ASSERT_PTR_NULL(file_operands_get_directory(NULL, stats, 0));
    CU_ASSERT_PTR_NULL(file_operands_get_directory(operands, NULL, 0));
}

void register_file_operands_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, test_teardown);

    if (suite == NULL)
        return ;

    CU_add_test(suite, "should_return_the_default_directory_for_zero_operands", should_return_the_default_directory_for_zero_operands);
    CU_add_test(suite, "should_copy_operands_in_order_and_skip_option_arguments", should_copy_operands_in_order_and_skip_option_arguments);
    CU_add_test(suite, "should_reject_missing_or_negative_operand_counts", should_reject_missing_or_negative_operand_counts);
    CU_add_test(suite, "should_retrieve_stats_with_or_without_following_symlinks", should_retrieve_stats_with_or_without_following_symlinks);
    CU_add_test(suite, "should_return_an_empty_stats_array_for_zero_operands", should_return_an_empty_stats_array_for_zero_operands);
    CU_add_test(suite, "should_reject_missing_or_negative_stats_inputs", should_reject_missing_or_negative_stats_inputs);
    CU_add_test(suite, "should_select_non_directory_operands_and_preserve_order", should_select_non_directory_operands_and_preserve_order);
    CU_add_test(suite, "should_select_directories_including_an_implicit_default_operand", should_select_directories_including_an_implicit_default_operand);
    CU_add_test(suite, "should_reject_null_operand_or_stats_arrays_when_selecting_categories", should_reject_null_operand_or_stats_arrays_when_selecting_categories);
}
