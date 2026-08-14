#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "mocks.h"
#include "file_data.h"

#define SUITE_NAME "file_data"

static void test_setup(void)
{
    reset_stat_guarantees();
}

static void should_return_an_error_when_retrieving_the_data_from_a_NULL_path(void)
{
    struct stat valid_stat;

    const int result = get_file_data(NULL, &valid_stat);

    CU_ASSERT_EQUAL(result, FILE_DATA_COULD_NOT_RETRIEVE_DATA);
}

static void should_return_an_error_when_retrieving_the_data_from_an_empty_path(void)
{
    struct stat valid_stat;

    const int result = get_file_data("", &valid_stat);

    CU_ASSERT_EQUAL(result, FILE_DATA_COULD_NOT_RETRIEVE_DATA);
}

static void should_return_an_error_when_populating_the_data_to_a_NULL_stat_struct(void)
{
    const int result = get_file_data("valid", NULL);

    CU_ASSERT_EQUAL(result, FILE_DATA_COULD_NOT_RETRIEVE_DATA);
}

static void should_populate_successfully_a_valid_struct_when_passed_a_valid_regular_file_path(void)
{
    guarantee_stat_will_populate_data_of_a_regular_type_file();
    struct stat valid_stat;

    const int result = get_file_data("valid", &valid_stat);

    CU_ASSERT_EQUAL(result, FILE_DATA_SUCCESS);
    CU_ASSERT_EQUAL(valid_stat.st_mode, S_IFREG);
}

static void should_populate_successfully_a_valid_struct_when_passed_a_valid_directory_path(void)
{
    guarantee_stat_will_populate_data_of_a_directory_type_file();
    struct stat valid_stat;

    const int result = get_file_data("valid", &valid_stat);

    CU_ASSERT_EQUAL(result, FILE_DATA_SUCCESS);
    CU_ASSERT_EQUAL(valid_stat.st_mode, S_IFDIR);
}

void register_file_data_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_return_an_error_when_retrieving_the_data_from_a_NULL_path", should_return_an_error_when_retrieving_the_data_from_a_NULL_path);
        CU_add_test(suite, "should_return_an_error_when_retrieving_the_data_from_an_empty_path", should_return_an_error_when_retrieving_the_data_from_an_empty_path);
        CU_add_test(suite, "should_return_an_error_when_populating_the_data_to_a_NULL_stat_struct", should_return_an_error_when_populating_the_data_to_a_NULL_stat_struct);
        CU_add_test(suite, "should_populate_successfully_a_valid_struct_when_passed_a_valid_regular_file_path", should_populate_successfully_a_valid_struct_when_passed_a_valid_regular_file_path);
        CU_add_test(suite, "should_populate_successfully_a_valid_struct_when_passed_a_valid_directory_path", should_populate_successfully_a_valid_struct_when_passed_a_valid_directory_path);
    }
}
