#include "file_data.h"

#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#define SUITE_NAME "file_data"
#define DEFAULT_NAME "default"

static t_file_data *sut;

static void initialize_file_data()
{
    sut = file_data_create(DEFAULT_NAME);
}

static void cleanup_file_data()
{
    file_data_destroy(&sut);
}

static t_file_data *create_next_file_data(const char *file_name)
{
    t_file_data *next = file_data_create(file_name);

    return next;
}

static void should_be_created_correctly(void)
{
    initialize_file_data();

    CU_ASSERT_PTR_NOT_NULL(sut);

    cleanup_file_data();
}

static void should_be_destroyed_correctly(void)
{
    initialize_file_data();

    cleanup_file_data();

    CU_ASSERT_PTR_NULL(sut);
}

static void should_fail_to_create_if_a_null_file_name_is_passed(void)
{
    const char *invalid_name = NULL;

    const t_file_data *invalid_file_data = create_next_file_data(invalid_name);

    CU_ASSERT_PTR_NULL(invalid_file_data);
}

static void should_fail_to_create_if_an_empty_file_name_is_passed(void)
{
    const char *invalid_name = "";

    const t_file_data *invalid_file_data = create_next_file_data(invalid_name);

    CU_ASSERT_PTR_NULL(invalid_file_data);
}

static void should_return_the_correct_name(void)
{
    initialize_file_data();

    const char *file_name = "valid_name";
    file_data_set_name(sut, file_name);

    const char *file_data_name = file_data_get_name(sut);

    CU_ASSERT_STRING_EQUAL(file_data_name, file_name);

    cleanup_file_data();
}

static void should_fail_setting_the_name_if_a_null_file_name_is_passed(void)
{
    initialize_file_data();

    const char *invalid_file_name = NULL;
    file_data_set_name(sut, invalid_file_name);

    const char *file_data_name = file_data_get_name(sut);

    CU_ASSERT_STRING_EQUAL(file_data_name, DEFAULT_NAME);
}

static void should_return_the_correct_length(void)
{
    initialize_file_data();

    file_data_add_entry(&sut, file_data_create(DEFAULT_NAME));
    file_data_add_entry(&sut, file_data_create(DEFAULT_NAME));

    CU_ASSERT_EQUAL(file_data_get_length(sut), 3);

    cleanup_file_data();
}

static void should_return_the_next_instance_correctly(void)
{
    initialize_file_data();

    const char *next_file_name = "next_file";
    t_file_data *next = create_next_file_data(next_file_name);
    file_data_add_entry(&sut, next);

    const char *file_name = file_data_get_name(file_data_get_next(sut));

    CU_ASSERT_STRING_EQUAL(file_name, next_file_name);

    cleanup_file_data();
}

void register_file_data_suite(void)
{
    const CU_pSuite suite = CU_add_suite(SUITE_NAME, NULL, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_be_created_correctly", should_be_created_correctly);
        CU_add_test(suite, "should_be_destroyed_correctly", should_be_destroyed_correctly);
        CU_add_test(suite, "should_return_the_correct_length", should_return_the_correct_length);
        CU_add_test(suite, "should_return_the_next_instance_correctly", should_return_the_next_instance_correctly);
        CU_add_test(suite, "should_fail_to_create_if_a_null_file_name_is_passed", should_fail_to_create_if_a_null_file_name_is_passed);
        CU_add_test(suite, "should_fail_to_create_if_an_empty_file_name_is_passed", should_fail_to_create_if_an_empty_file_name_is_passed);
        CU_add_test(suite, "should_return_the_correct_name", should_return_the_correct_name);
        CU_add_test(suite, "should_fail_setting_the_name_if_a_null_file_name_is_passed", should_fail_setting_the_name_if_a_null_file_name_is_passed);
    }
}
