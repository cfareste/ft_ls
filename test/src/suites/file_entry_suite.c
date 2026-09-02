#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "file_entry.h"

#define SUITE_NAME "file_entry"
#define DEFAULT_NAME "default"

static t_file_entry_array *sut;

static void test_setup(void)
{
    sut = file_entry_array_create();
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
}

static void should_fail_to_create_an_entry_if_a_NULL_file_name_is_passed(void)
{
    const t_file_entry *invalid = file_entry_create(NULL);

    CU_ASSERT_PTR_NULL(invalid);
}

static void should_fail_to_create_an_entry_if_an_empty_file_name_is_passed(void)
{
    const t_file_entry *invalid = file_entry_create("");

    CU_ASSERT_PTR_NULL(invalid);
}

static void should_destroy_file_entry_correctly(void)
{
    t_file_entry *entry = file_entry_create("valid");

    file_entry_destroy(&entry);

    CU_ASSERT_PTR_NULL(entry);
}

static void should_destroy_file_entry_array_correctly(void)
{
    file_entry_array_push(sut, file_entry_create("entry"));
    file_entry_array_push(sut, file_entry_create("entry2"));

    file_entry_array_destroy(&sut);

    assert_file_entry_array_is_null(sut);
}

static void should_not_fail_to_destroy_a_file_entry_when_a_null_pointer_is_passed(void)
{
    file_entry_destroy(NULL);
}

static void should_not_fail_to_destroy_a_file_entry_that_is_already_null(void)
{
    t_file_entry *entry = NULL;

    file_entry_destroy(&entry);
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

static void should_return_NULL_for_the_entry_name_if_a_NULL_entry_is_passed(void)
{
    const char *entry_name = file_entry_get_name(NULL);

    CU_ASSERT_PTR_NULL(entry_name);
}

static void should_not_fail_setting_the_entry_name_if_a_NULL_file_entry_is_passed(void)
{
    file_entry_set_name(NULL, "name");
}

static void should_not_set_the_entry_name_if_a_NULL_file_name_is_passed(void)
{
    t_file_entry *entry = file_entry_create("valid");
    file_entry_set_name(entry, NULL);

    const char *entry_name = file_entry_get_name(entry);

    CU_ASSERT_STRING_EQUAL(entry_name, "valid");

    file_entry_destroy(&entry);
}

static void should_not_set_the_entry_name_if_an_empty_file_name_is_passed(void)
{
    t_file_entry *entry = file_entry_create("valid");
    file_entry_set_name(entry, "");

    const char *entry_name = file_entry_get_name(entry);

    CU_ASSERT_STRING_EQUAL(entry_name, "valid");

    file_entry_destroy(&entry);
}

static void should_return_the_entry_name_correctly(void)
{
    t_file_entry *file_entry = file_entry_create("name");
    file_entry_set_name(file_entry, "changed name");

    const char *entry_name = file_entry_get_name(file_entry);

    CU_ASSERT_STRING_EQUAL(entry_name, "changed name");

    file_entry_destroy(&file_entry);
}

static void should_return_a_length_of_zero_when_passed_a_NULL_file_entry_array(void)
{
    const unsigned int length = file_entry_array_get_length(NULL);

    CU_ASSERT_EQUAL(length, 0);
}

static void should_return_the_correct_length(void)
{
    file_entry_array_push(sut, file_entry_create(DEFAULT_NAME));
    file_entry_array_push(sut, file_entry_create(DEFAULT_NAME));

    CU_ASSERT_EQUAL(file_entry_array_get_length(sut), 2);
}

static void should_return_NULL_for_an_entry_if_a_NULL_array_is_passed(void)
{
    const t_file_entry *entry = file_entry_array_get_at(NULL, 0);

    CU_ASSERT_PTR_NULL(entry);
}

static void should_return_NULL_for_an_entry_if_an_out_of_bounds_index_is_specified(void)
{
    file_entry_array_push(sut, file_entry_create("entry 0"));
    file_entry_array_push(sut, file_entry_create("entry 1"));
    file_entry_array_push(sut, file_entry_create("entry 2"));

    CU_ASSERT_PTR_NULL(file_entry_array_get_at(sut, 3));
    CU_ASSERT_PTR_NULL(file_entry_array_get_at(sut, 17));
    CU_ASSERT_PTR_NULL(file_entry_array_get_at(sut, -1));
}

static void should_return_the_entry_at_the_specified_index(void)
{
    file_entry_array_push(sut, file_entry_create("entry 0"));
    file_entry_array_push(sut, file_entry_create("entry 1"));
    file_entry_array_push(sut, file_entry_create("entry 2"));
    file_entry_array_push(sut, file_entry_create("entry 3"));

    const t_file_entry *first_entry = file_entry_array_get_at(sut, 0);
    const t_file_entry *second_entry = file_entry_array_get_at(sut, 1);
    const t_file_entry *third_entry = file_entry_array_get_at(sut, 2);
    const t_file_entry *fourth_entry = file_entry_array_get_at(sut, 3);
    const char *first_entry_name = file_entry_get_name(first_entry);
    const char *second_entry_name = file_entry_get_name(second_entry);
    const char *third_entry_name = file_entry_get_name(third_entry);
    const char *fourth_entry_name = file_entry_get_name(fourth_entry);

    CU_ASSERT_STRING_EQUAL(first_entry_name, "entry 0");
    CU_ASSERT_STRING_EQUAL(second_entry_name, "entry 1");
    CU_ASSERT_STRING_EQUAL(third_entry_name, "entry 2");
    CU_ASSERT_STRING_EQUAL(fourth_entry_name, "entry 3");
}

static void should_not_fail_pushing_an_entry_to_a_NULL_array(void)
{
    t_file_entry *entry = file_entry_create("valid");

    file_entry_array_push(NULL, entry);

    file_entry_destroy(&entry);
}

static void should_not_fail_pushing_a_NULL_entry_to_an_array(void)
{
    file_entry_array_push(sut, NULL);
}

static void should_push_correctly_an_specified_entry(void)
{
    for (unsigned int i = 0; i < 10; i++)
    {
        file_entry_array_push(sut, file_entry_create("entry"));
    }

    unsigned int i = 0;
    for (; i < 10; i++)
    {
        CU_ASSERT_PTR_NOT_NULL(file_entry_array_get_at(sut, i));
    }
    CU_ASSERT_PTR_NULL(file_entry_array_get_at(sut, i));
}

static void should_not_fail_when_sorting_a_NULL_file_entry_array(void)
{
    file_entry_array_sort(NULL);
}

static void should_sort_the_file_operands_by_entry_name(void)
{
    const char *entry_names[] = { "2file", ".hiddir", "_file", "dir", "FILE", NULL };
    file_entry_array_push(sut, file_entry_create(entry_names[0]));
    file_entry_array_push(sut, file_entry_create(entry_names[1]));
    file_entry_array_push(sut, file_entry_create(entry_names[2]));
    file_entry_array_push(sut, file_entry_create(entry_names[3]));
    file_entry_array_push(sut, file_entry_create(entry_names[4]));

    file_entry_array_sort(sut);
    const t_file_entry *first_entry = file_entry_array_get_at(sut, 0);
    const t_file_entry *second_entry = file_entry_array_get_at(sut, 1);
    const t_file_entry *third_entry = file_entry_array_get_at(sut, 2);
    const t_file_entry *fourth_entry = file_entry_array_get_at(sut, 3);
    const t_file_entry *fifth_entry = file_entry_array_get_at(sut, 4);

    CU_ASSERT_STRING_EQUAL(file_entry_get_name(first_entry), entry_names[1]);
    CU_ASSERT_STRING_EQUAL(file_entry_get_name(second_entry), entry_names[0]);
    CU_ASSERT_STRING_EQUAL(file_entry_get_name(third_entry), entry_names[4]);
    CU_ASSERT_STRING_EQUAL(file_entry_get_name(fourth_entry), entry_names[2]);
    CU_ASSERT_STRING_EQUAL(file_entry_get_name(fifth_entry), entry_names[3]);
    CU_ASSERT_PTR_NULL(file_entry_array_get_at(sut, 5));
}

static void should_manage_correctly_the_max_capacity(void)
{
    for (unsigned int i = 0; i < 8; i++)
    {
        file_entry_array_push(sut, file_entry_create("entry"));
    }

    file_entry_array_sort(sut);
}
void register_file_entry_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, test_teardown);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_create_file_entry_correctly", should_create_file_entry_correctly);
        CU_add_test(suite, "should_create_file_entry_array_correctly", should_create_file_entry_array_correctly);
        CU_add_test(suite, "should_fail_to_create_an_entry_if_a_NULL_file_name_is_passed", should_fail_to_create_an_entry_if_a_NULL_file_name_is_passed);
        CU_add_test(suite, "should_fail_to_create_an_entry_if_an_empty_file_name_is_passed", should_fail_to_create_an_entry_if_an_empty_file_name_is_passed);
        CU_add_test(suite, "should_destroy_file_entry_correctly", should_destroy_file_entry_correctly);
        CU_add_test(suite, "should_destroy_file_entry_array_correctly", should_destroy_file_entry_array_correctly);
        CU_add_test(suite, "should_not_fail_to_destroy_a_file_entry_when_a_null_pointer_is_passed", should_not_fail_to_destroy_a_file_entry_when_a_null_pointer_is_passed);
        CU_add_test(suite, "should_not_fail_to_destroy_a_file_entry_that_is_already_null", should_not_fail_to_destroy_a_file_entry_that_is_already_null);
        CU_add_test(suite, "should_not_fail_to_destroy_a_file_entry_array_when_a_null_pointer_is_passed", should_not_fail_to_destroy_a_file_entry_array_when_a_null_pointer_is_passed);
        CU_add_test(suite, "should_not_fail_to_destroy_a_file_entry_array_that_is_already_null", should_not_fail_to_destroy_a_file_entry_array_that_is_already_null);
        CU_add_test(suite, "should_return_NULL_for_the_entry_name_if_a_NULL_entry_is_passed", should_return_NULL_for_the_entry_name_if_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_not_fail_setting_the_entry_name_if_a_NULL_file_entry_is_passed", should_not_fail_setting_the_entry_name_if_a_NULL_file_entry_is_passed);
        CU_add_test(suite, "should_not_set_the_entry_name_if_a_NULL_file_name_is_passed", should_not_set_the_entry_name_if_a_NULL_file_name_is_passed);
        CU_add_test(suite, "should_not_set_the_entry_name_if_an_empty_file_name_is_passed", should_not_set_the_entry_name_if_an_empty_file_name_is_passed);
        CU_add_test(suite, "should_return_the_entry_name_correctly", should_return_the_entry_name_correctly);
        CU_add_test(suite, "should_return_a_length_of_zero_when_passed_a_NULL_file_entry_array", should_return_a_length_of_zero_when_passed_a_NULL_file_entry_array);
        CU_add_test(suite, "should_return_the_correct_length", should_return_the_correct_length);
        CU_add_test(suite, "should_return_NULL_for_an_entry_if_a_NULL_array_is_passed", should_return_NULL_for_an_entry_if_a_NULL_array_is_passed);
        CU_add_test(suite, "should_return_NULL_for_an_entry_if_an_out_of_bounds_index_is_specified", should_return_NULL_for_an_entry_if_an_out_of_bounds_index_is_specified);
        CU_add_test(suite, "should_return_the_entry_at_the_specified_index", should_return_the_entry_at_the_specified_index);
        CU_add_test(suite, "should_not_fail_pushing_an_entry_to_a_NULL_array", should_not_fail_pushing_an_entry_to_a_NULL_array);
        CU_add_test(suite, "should_not_fail_pushing_a_NULL_entry_to_an_array", should_not_fail_pushing_a_NULL_entry_to_an_array);
        CU_add_test(suite, "should_push_correctly_an_specified_entry", should_push_correctly_an_specified_entry);
        CU_add_test(suite, "should_not_fail_when_sorting_a_NULL_file_entry_array", should_not_fail_when_sorting_a_NULL_file_entry_array);
        CU_add_test(suite, "should_sort_the_file_operands_by_ascii", should_sort_the_file_operands_by_entry_name);
        CU_add_test(suite, "should_manage_correctly_the_max_capacity", should_manage_correctly_the_max_capacity);
    }
}
