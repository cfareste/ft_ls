#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "file_data.h"

#define SUITE_NAME "file_data"
#define DEFAULT_NAME "default"

static t_file_data *sut;

static void initialize_file_data()
{
    sut = file_data_create(DEFAULT_NAME);
}

static void test_setup(void)
{
    initialize_file_data();
}

static void test_teardown(void)
{
    file_data_destroy(&sut);
}

static void assert_file_data_is_null(const t_file_data *file_data)
{
    CU_ASSERT_PTR_NULL(file_data);
}

static void should_be_created_correctly(void)
{
    CU_ASSERT_PTR_NOT_NULL(sut);
    CU_ASSERT_STRING_EQUAL(file_data_get_name(sut), DEFAULT_NAME);
}

static void should_be_destroyed_correctly(void)
{
    file_data_destroy(&sut);

    assert_file_data_is_null(sut);
}

static void should_not_fail_to_destroy_a_file_data_when_a_null_pointer_is_passed(void)
{
    file_data_destroy(NULL);
}

static void should_not_fail_to_destroy_a_file_data_that_is_already_null(void)
{
    file_data_destroy(&sut);
    file_data_destroy(&sut);
}

static void should_fail_to_create_if_a_null_file_name_is_passed(void)
{
    const char *invalid_name = NULL;

    const t_file_data *invalid_file_data = file_data_create(invalid_name);

    assert_file_data_is_null(invalid_file_data);
}

static void should_fail_to_create_if_an_empty_file_name_is_passed(void)
{
    const char *invalid_name = "";

    const t_file_data *invalid_file_data = file_data_create(invalid_name);

    assert_file_data_is_null(invalid_file_data);
}

static void should_return_NULL_if_a_null_file_data_is_passed_the_correct_name(void)
{
    const char *file_data_name = file_data_get_name(NULL);

    CU_ASSERT_PTR_NULL(file_data_name);
}

static void should_return_the_correct_name(void)
{
    const char *file_name = "valid_name";
    file_data_set_name(sut, file_name);

    const char *file_data_name = file_data_get_name(sut);

    CU_ASSERT_STRING_EQUAL(file_data_name, file_name);
}

static void should_not_fail_setting_the_name_if_a_null_file_data_is_passed(void)
{
    file_data_set_name(NULL, "name");
}

static void should_not_fail_setting_the_name_if_a_null_file_name_is_passed(void)
{
    const char *invalid_file_name = NULL;
    file_data_set_name(sut, invalid_file_name);

    const char *file_data_name = file_data_get_name(sut);

    CU_ASSERT_STRING_EQUAL(file_data_name, DEFAULT_NAME);
}

static void should_not_fail_setting_the_name_if_an_empty_file_name_is_passed(void)
{
    const char *invalid_file_name = "";
    file_data_set_name(sut, invalid_file_name);

    const char *file_data_name = file_data_get_name(sut);

    CU_ASSERT_STRING_EQUAL(file_data_name, DEFAULT_NAME);
}

static void should_return_length_of_zero_when_passed_a_null_file_data(void)
{
    CU_ASSERT_EQUAL(file_data_get_length(NULL), 0);
}

static void should_return_the_correct_length(void)
{
    file_data_add_entry(&sut, file_data_create(DEFAULT_NAME));
    file_data_add_entry(&sut, file_data_create(DEFAULT_NAME));

    CU_ASSERT_EQUAL(file_data_get_length(sut), 3);
}

static void should_not_fail_returning_the_next_instance_if_a_null_file_data_is_passed(void)
{
    file_data_get_next(NULL);
}

static void should_not_fail_when_attempting_to_add_a_next_entry_to_a_null_pointer(void)
{
    file_data_add_entry(NULL, sut);
}

static void should_add_the_next_instance_as_the_first_element_if_a_null_file_data_is_passed(void)
{
    t_file_data *list = NULL;
    const char *sut_name = "first_element";
    file_data_set_name(sut, sut_name);

    file_data_add_entry(&list, sut);
    const char *file_name = file_data_get_name(list);

    CU_ASSERT_STRING_EQUAL(file_name, sut_name);
}

static void should_return_the_next_instance_correctly(void)
{
    const char *next_file_name = "next_file";
    t_file_data *next = file_data_create(next_file_name);
    file_data_add_entry(&sut, next);

    const t_file_data *sut_next = file_data_get_next(sut);
    const char *file_name = file_data_get_name(sut_next);

    CU_ASSERT_STRING_EQUAL(file_name, next_file_name);
}

void register_file_data_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, test_teardown);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_be_created_correctly", should_be_created_correctly);
        CU_add_test(suite, "should_be_destroyed_correctly", should_be_destroyed_correctly);
        CU_add_test(suite, "should_not_fail_to_destroy_a_file_data_when_a_null_pointer_is_passed", should_not_fail_to_destroy_a_file_data_when_a_null_pointer_is_passed);
        CU_add_test(suite, "should_not_fail_to_destroy_a_file_data_that_is_already_null", should_not_fail_to_destroy_a_file_data_that_is_already_null);
        CU_add_test(suite, "should_fail_to_create_if_a_null_file_name_is_passed", should_fail_to_create_if_a_null_file_name_is_passed);
        CU_add_test(suite, "should_fail_to_create_if_an_empty_file_name_is_passed", should_fail_to_create_if_an_empty_file_name_is_passed);
        CU_add_test(suite, "should_return_NULL_if_a_null_file_data_is_passed_the_correct_name", should_return_NULL_if_a_null_file_data_is_passed_the_correct_name);
        CU_add_test(suite, "should_return_length_of_zero_when_passed_a_null_file_data", should_return_length_of_zero_when_passed_a_null_file_data);
        CU_add_test(suite, "should_return_the_correct_name", should_return_the_correct_name);
        CU_add_test(suite, "should_not_fail_setting_the_name_if_a_null_file_data_is_passed", should_not_fail_setting_the_name_if_a_null_file_data_is_passed);
        CU_add_test(suite, "should_not_fail_setting_the_name_if_a_null_file_name_is_passed", should_not_fail_setting_the_name_if_a_null_file_name_is_passed);
        CU_add_test(suite, "should_not_fail_setting_the_name_if_an_empty_file_name_is_passed", should_not_fail_setting_the_name_if_an_empty_file_name_is_passed);
        CU_add_test(suite, "should_return_the_correct_length", should_return_the_correct_length);
        CU_add_test(suite, "should_not_fail_returning_the_next_instance_if_a_null_file_data_is_passed", should_not_fail_returning_the_next_instance_if_a_null_file_data_is_passed);
        CU_add_test(suite, "should_not_fail_when_attempting_to_add_a_next_entry_to_a_null_pointer", should_not_fail_when_attempting_to_add_a_next_entry_to_a_null_pointer);
        CU_add_test(suite, "should_add_the_next_instance_as_the_first_element_if_a_null_file_data_is_passed", should_add_the_next_instance_as_the_first_element_if_a_null_file_data_is_passed);
        CU_add_test(suite, "should_return_the_next_instance_correctly", should_return_the_next_instance_correctly);
    }
}
