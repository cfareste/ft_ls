#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "mocks.h"
#include "file_stats.h"

#define SUITE_NAME "file_stats"

static struct stat file_stats;

static void test_setup(void)
{
    vfs_mock_reset();
    reset_stat_guarantees();
}

static void assert_file_stats_retrieving_failed(const int result)
{
    CU_ASSERT_EQUAL(result, FILE_STATS_COULD_NOT_RETRIEVE_STATS);
}

static void assert_file_stats_retrieving_succeed(const int result)
{
    CU_ASSERT_EQUAL(result, FILE_STATS_SUCCESS);
}

static void assert_file_type_is(const unsigned int type)
{
    CU_ASSERT_EQUAL(file_stats.st_mode & S_IFMT, type);
}

static void should_return_an_error_when_retrieving_the_stats_from_a_NULL_path(void)
{
    const int result = file_stats_get(NULL, &file_stats);

    assert_file_stats_retrieving_failed(result);
}

static void should_return_an_error_when_retrieving_the_stats_from_an_empty_path(void)
{
    const int result = file_stats_get("", &file_stats);

    assert_file_stats_retrieving_failed(result);
}

static void should_return_an_error_when_populating_the_stats_to_a_NULL_stat_struct(void)
{
    const int result = file_stats_get("valid", NULL);

    assert_file_stats_retrieving_failed(result);
}

static void should_populate_successfully_the_file_stats_when_passed_a_valid_regular_file_path(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("valid_file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const int result = file_stats_get("valid_file", &file_stats);

    assert_file_stats_retrieving_succeed(result);
    assert_file_type_is(S_IFREG);
}

static void should_populate_successfully_the_file_stats_when_passed_a_valid_directory_path(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("valid_dir", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const int result = file_stats_get("valid_dir", &file_stats);

    assert_file_stats_retrieving_succeed(result);
    assert_file_type_is(S_IFDIR);
}

void register_file_stats_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_return_an_error_when_retrieving_the_stats_from_a_NULL_path", should_return_an_error_when_retrieving_the_stats_from_a_NULL_path);
        CU_add_test(suite, "should_return_an_error_when_retrieving_the_stats_from_an_empty_path", should_return_an_error_when_retrieving_the_stats_from_an_empty_path);
        CU_add_test(suite, "should_return_an_error_when_populating_the_stats_to_a_NULL_stat_struct", should_return_an_error_when_populating_the_stats_to_a_NULL_stat_struct);
        CU_add_test(suite, "should_populate_successfully_the_file_stats_when_passed_a_valid_regular_file_path", should_populate_successfully_the_file_stats_when_passed_a_valid_regular_file_path);
        CU_add_test(suite, "should_populate_successfully_the_file_stats_when_passed_a_valid_directory_path", should_populate_successfully_the_file_stats_when_passed_a_valid_directory_path);
    }
}
