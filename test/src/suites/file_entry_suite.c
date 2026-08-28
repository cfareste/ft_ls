#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "file_entry.h"

#define SUITE_NAME "file_entry"
#define DEFAULT_NAME "default"

static t_file_entry_array *sut;

static void initialize_file_entry_array()
{
    sut = file_entry_array_create(DEFAULT_NAME);
}

static void test_setup(void)
{
    initialize_file_entry_array();
}

static void test_teardown(void)
{
    file_entry_array_destroy(&sut);
}

static void assert_file_entry_array_is_null(const t_file_entry_array *file_entry_array)
{
    CU_ASSERT_PTR_NULL(file_entry_array);
}

static void should_create_file_entry_correctly(void)
{
    t_file_entry *file_entry = file_entry_create(DEFAULT_NAME);

    CU_ASSERT_PTR_NOT_NULL(file_entry);
    CU_ASSERT_STRING_EQUAL(file_entry_get_name(file_entry), DEFAULT_NAME);

    file_entry_destroy(&file_entry);
}

static void should_create_file_entry_array_correctly(void)
{
    CU_ASSERT_PTR_NOT_NULL(sut);
    CU_ASSERT_STRING_EQUAL(file_entry_array_get_name(sut), DEFAULT_NAME);
}

static void should_destroy_file_entry_array_correctly(void)
{
    file_entry_array_destroy(&sut);

    assert_file_entry_array_is_null(sut);
}

static void should_not_fail_to_destroy_a_file_entry_array_when_a_null_pointer_is_passed(void)
{
    file_entry_array_destroy(NULL);
}

static void should_not_fail_to_destroy_a_file_entry_array_that_is_already_null(void)
{
    file_entry_array_destroy(&sut);
    file_entry_array_destroy(&sut);
}

static void should_fail_to_create_if_a_null_file_name_is_passed(void)
{
    const char *invalid_name = NULL;

    const t_file_entry_array *invalid_file_entry_array = file_entry_array_create(invalid_name);

    assert_file_entry_array_is_null(invalid_file_entry_array);
}

static void should_fail_to_create_if_an_empty_file_name_is_passed(void)
{
    const char *invalid_name = "";

    const t_file_entry_array *invalid_file_entry_array = file_entry_array_create(invalid_name);

    assert_file_entry_array_is_null(invalid_file_entry_array);
}

static void should_return_NULL_if_a_null_file_entry_array_is_passed_the_correct_name(void)
{
    const char *entry_name = file_entry_array_get_name(NULL);

    CU_ASSERT_PTR_NULL(entry_name);
}

static void should_return_the_correct_name(void)
{
    const char *file_name = "valid_name";
    file_entry_array_set_name(sut, file_name);

    const char *entry_name = file_entry_array_get_name(sut);

    CU_ASSERT_STRING_EQUAL(entry_name, file_name);
}

static void should_not_fail_setting_the_name_if_a_null_file_entry_array_is_passed(void)
{
    file_entry_array_set_name(NULL, "name");
}

static void should_not_fail_setting_the_name_if_a_null_file_name_is_passed(void)
{
    const char *invalid_file_name = NULL;
    file_entry_array_set_name(sut, invalid_file_name);

    const char *entry_name = file_entry_array_get_name(sut);

    CU_ASSERT_STRING_EQUAL(entry_name, DEFAULT_NAME);
}

static void should_not_fail_setting_the_name_if_an_empty_file_name_is_passed(void)
{
    const char *invalid_file_name = "";
    file_entry_array_set_name(sut, invalid_file_name);

    const char *entry_name = file_entry_array_get_name(sut);

    CU_ASSERT_STRING_EQUAL(entry_name, DEFAULT_NAME);
}

static void should_return_length_of_zero_when_passed_a_null_file_entry_array(void)
{
    CU_ASSERT_EQUAL(file_entry_array_get_length(NULL), 0);
}

static void should_return_the_correct_length(void)
{
    file_entry_array_push(&sut, file_entry_array_create(DEFAULT_NAME));
    file_entry_array_push(&sut, file_entry_array_create(DEFAULT_NAME));

    CU_ASSERT_EQUAL(file_entry_array_get_length(sut), 3);
}

static void should_not_fail_returning_the_next_instance_if_a_null_file_entry_array_is_passed(void)
{
    file_entry_array_get_next(NULL);
}

static void should_not_fail_when_attempting_to_push_an_entry_to_a_null_pointer(void)
{
    file_entry_array_push(NULL, sut);
}

static void should_push_the_next_instance_as_the_first_element_if_a_null_file_entry_array_is_passed(void)
{
    t_file_entry_array *array = NULL;
    const char *sut_name = "first_element";
    file_entry_array_set_name(sut, sut_name);

    file_entry_array_push(&array, sut);
    const char *file_name = file_entry_array_get_name(array);

    CU_ASSERT_STRING_EQUAL(file_name, sut_name);
}

static void should_push_an_entry_to_the_array_correctly(void)
{
    t_file_entry *first_entry = file_entry_create("valid entry");
    t_file_entry *second_entry = file_entry_create("valid entry 2");
    t_file_entry *third_entry = file_entry_create("valid entry 3");
    t_file_entry_array *array = file_entry_array_create("array temp");

    file_entry_array_push_TEMP(array, first_entry);
    file_entry_array_push_TEMP(array, second_entry);
    file_entry_array_push_TEMP(array, third_entry);
    const char *first_entry_name = file_entry_get_name(file_entry_array_get_at(array, 0));
    const char *second_entry_name = file_entry_get_name(file_entry_array_get_at(array, 1));
    const char *third_entry_name = file_entry_get_name(file_entry_array_get_at(array, 2));

    CU_ASSERT_STRING_EQUAL(first_entry_name, "valid entry");
    CU_ASSERT_STRING_EQUAL(second_entry_name, "valid entry 2");
    CU_ASSERT_STRING_EQUAL(third_entry_name, "valid entry 3");

    file_entry_array_destroy(&array);
}

static void should_return_the_next_instance_correctly(void)
{
    const char *next_file_name = "next_file";
    t_file_entry_array *next = file_entry_array_create(next_file_name);
    file_entry_array_push(&sut, next);

    const t_file_entry_array *sut_next = file_entry_array_get_next(sut);
    const char *file_name = file_entry_array_get_name(sut_next);

    CU_ASSERT_STRING_EQUAL(file_name, next_file_name);
}

void register_file_entry_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, test_teardown);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_create_file_entry_correctly", should_create_file_entry_correctly);
        CU_add_test(suite, "should_create_file_entry_array_correctly", should_create_file_entry_array_correctly);
        CU_add_test(suite, "should_destroy_file_entry_array_correctly", should_destroy_file_entry_array_correctly);
        CU_add_test(suite, "should_not_fail_to_destroy_a_file_entry_array_when_a_null_pointer_is_passed", should_not_fail_to_destroy_a_file_entry_array_when_a_null_pointer_is_passed);
        CU_add_test(suite, "should_not_fail_to_destroy_a_file_entry_array_that_is_already_null", should_not_fail_to_destroy_a_file_entry_array_that_is_already_null);
        CU_add_test(suite, "should_fail_to_create_if_a_null_file_name_is_passed", should_fail_to_create_if_a_null_file_name_is_passed);
        CU_add_test(suite, "should_fail_to_create_if_an_empty_file_name_is_passed", should_fail_to_create_if_an_empty_file_name_is_passed);
        CU_add_test(suite, "should_return_NULL_if_a_null_file_entry_array_is_passed_the_correct_name", should_return_NULL_if_a_null_file_entry_array_is_passed_the_correct_name);
        CU_add_test(suite, "should_return_length_of_zero_when_passed_a_null_file_entry_array", should_return_length_of_zero_when_passed_a_null_file_entry_array);
        CU_add_test(suite, "should_return_the_correct_name", should_return_the_correct_name);
        CU_add_test(suite, "should_not_fail_setting_the_name_if_a_null_file_entry_array_is_passed", should_not_fail_setting_the_name_if_a_null_file_entry_array_is_passed);
        CU_add_test(suite, "should_not_fail_setting_the_name_if_a_null_file_name_is_passed", should_not_fail_setting_the_name_if_a_null_file_name_is_passed);
        CU_add_test(suite, "should_not_fail_setting_the_name_if_an_empty_file_name_is_passed", should_not_fail_setting_the_name_if_an_empty_file_name_is_passed);
        CU_add_test(suite, "should_return_the_correct_length", should_return_the_correct_length);
        CU_add_test(suite, "should_not_fail_returning_the_next_instance_if_a_null_file_entry_array_is_passed", should_not_fail_returning_the_next_instance_if_a_null_file_entry_array_is_passed);
        CU_add_test(suite, "should_not_fail_when_attempting_to_push_an_entry_to_a_null_pointer", should_not_fail_when_attempting_to_push_an_entry_to_a_null_pointer);
        CU_add_test(suite, "should_push_an_entry_to_the_array_correctly", should_push_an_entry_to_the_array_correctly);
        CU_add_test(suite, "should_push_the_next_instance_as_the_first_element_if_a_null_file_entry_array_is_passed", should_push_the_next_instance_as_the_first_element_if_a_null_file_entry_array_is_passed);
        CU_add_test(suite, "should_return_the_next_instance_correctly", should_return_the_next_instance_correctly);
    }
}
