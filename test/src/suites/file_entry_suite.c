#include "suites.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "file_entry.h"

#define SUITE_NAME "file_entry"
#define DEFAULT_NAME "default"
#define DEFAULT_TYPE FILE_TYPE_UNKNOWN

static t_file_entry *entry_sut;
static t_file_entry_array *array_sut;

static void test_setup(void)
{
    entry_sut = file_entry_create(DEFAULT_NAME);
    array_sut = file_entry_array_create();
}

static void test_teardown(void)
{
    file_entry_destroy(&entry_sut);
    file_entry_array_destroy(&array_sut);
}

static void assert_file_entry_array_is_null(const t_file_entry_array *file_entry_array)
{
    CU_ASSERT_PTR_NULL(file_entry_array);
}

static void should_create_file_entry_correctly(void)
{
    CU_ASSERT_PTR_NOT_NULL(entry_sut);
    CU_ASSERT_STRING_EQUAL(file_entry_get_name(entry_sut), DEFAULT_NAME);
    CU_ASSERT_EQUAL(file_entry_get_file_type(entry_sut), DEFAULT_TYPE);
}

static void should_create_file_entry_array_correctly(void)
{
    CU_ASSERT_PTR_NOT_NULL(array_sut);
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
    file_entry_destroy(&entry_sut);

    CU_ASSERT_PTR_NULL(entry_sut);
}

static void should_destroy_file_entry_array_correctly(void)
{
    file_entry_array_push(array_sut, file_entry_create("entry"));
    file_entry_array_push(array_sut, file_entry_create("entry2"));

    file_entry_array_destroy(&array_sut);

    assert_file_entry_array_is_null(array_sut);
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
    file_entry_array_destroy(&array_sut);
    file_entry_array_destroy(&array_sut);
}

static void should_return_NULL_for_the_entry_name_if_a_NULL_entry_is_passed(void)
{
    const char *entry_name = file_entry_get_name(NULL);

    CU_ASSERT_PTR_NULL(entry_name);
}

static void should_return_the_entry_name_correctly(void)
{
    file_entry_set_name(entry_sut, "changed name");

    const char *entry_name = file_entry_get_name(entry_sut);

    CU_ASSERT_STRING_EQUAL(entry_name, "changed name");
}

static void should_not_fail_setting_the_entry_name_if_a_NULL_file_entry_is_passed(void)
{
    file_entry_set_name(NULL, "name");
}

static void should_not_set_the_entry_name_if_a_NULL_file_name_is_passed(void)
{
    file_entry_set_name(entry_sut, NULL);

    const char *entry_name = file_entry_get_name(entry_sut);

    CU_ASSERT_STRING_EQUAL(entry_name, DEFAULT_NAME);
}

static void should_not_set_the_entry_name_if_an_empty_file_name_is_passed(void)
{
    file_entry_set_name(entry_sut, "");

    const char *entry_name = file_entry_get_name(entry_sut);

    CU_ASSERT_STRING_EQUAL(entry_name, DEFAULT_NAME);
}

static void should_return_zero_blocks_when_a_NULL_entry_is_passed(void)
{
    CU_ASSERT_EQUAL(file_entry_get_blocks(NULL), 0);
}

static void should_return_the_entry_blocks_correctly(void)
{
    file_entry_set_blocks(entry_sut, 17);

    CU_ASSERT_EQUAL(file_entry_get_blocks(entry_sut), 17);
}

static void should_not_fail_setting_entry_blocks_if_a_NULL_entry_is_passed(void)
{
    file_entry_set_blocks(NULL, 17);
}

static void should_return_no_permissions_when_a_NULL_entry_is_passed(void)
{
    CU_ASSERT_EQUAL(file_entry_get_permissions(NULL), PERM_NONE);
}

static void should_return_the_entry_permissions_correctly(void)
{
    const t_permissions permissions = PERM_OWNER_READ | PERM_GROUP_WRITE;

    file_entry_set_permissions(entry_sut, permissions);

    CU_ASSERT_EQUAL(file_entry_get_permissions(entry_sut), permissions);
}

static void should_not_fail_setting_entry_permissions_if_a_NULL_entry_is_passed(void)
{
    file_entry_set_permissions(NULL, PERM_OWNER_READ);
}

static void should_return_unknown_for_the_entry_file_type_if_a_NULL_entry_is_passed(void)
{
    const t_file_type file_type = file_entry_get_file_type(NULL);

    CU_ASSERT_EQUAL(file_type, FILE_TYPE_UNKNOWN);
}

static void should_return_the_entry_file_type_correctly(void)
{
    file_entry_set_file_type(entry_sut, FILE_TYPE_REGULAR);

    const t_file_type entry_file_type = file_entry_get_file_type(entry_sut);

    CU_ASSERT_EQUAL(entry_file_type, FILE_TYPE_REGULAR);
}

static void should_not_fail_setting_the_entry_file_type_if_a_NULL_file_entry_is_passed(void)
{
    file_entry_set_file_type(NULL, FILE_TYPE_REGULAR);
}

static void should_not_set_the_entry_file_type_if_none_file_type_is_passed(void)
{
    file_entry_set_file_type(entry_sut, FILE_TYPE_NONE);

    const t_file_type file_type = file_entry_get_file_type(entry_sut);

    CU_ASSERT_EQUAL(file_type, FILE_TYPE_UNKNOWN);
}

static void should_return_zero_link_count_when_a_NULL_entry_is_passed(void)
{
    CU_ASSERT_EQUAL(file_entry_get_link_count(NULL), 0);
}

static void should_return_the_entry_link_count_correctly(void)
{
    file_entry_set_link_count(entry_sut, 17);

    CU_ASSERT_EQUAL(file_entry_get_link_count(entry_sut), 17);
}

static void should_not_fail_setting_entry_link_count_if_a_NULL_entry_is_passed(void)
{
    file_entry_set_link_count(NULL, 17);
}

static void should_return_NULL_for_the_entry_author_if_a_NULL_entry_is_passed(void)
{
    CU_ASSERT_PTR_NULL(file_entry_get_author(NULL));
}

static void should_return_the_entry_author_correctly(void)
{
    file_entry_set_author(entry_sut, "alice");

    CU_ASSERT_STRING_EQUAL(file_entry_get_author(entry_sut), "alice");
}

static void should_not_set_the_entry_author_if_a_NULL_author_is_passed(void)
{
    file_entry_set_author(entry_sut, "alice");
    file_entry_set_author(entry_sut, NULL);

    CU_ASSERT_STRING_EQUAL(file_entry_get_author(entry_sut), "alice");
}

static void should_not_fail_setting_the_entry_author_if_a_NULL_entry_is_passed(void)
{
    file_entry_set_author(NULL, "alice");
}

static void should_return_NULL_for_the_entry_group_if_a_NULL_entry_is_passed(void)
{
    CU_ASSERT_PTR_NULL(file_entry_get_group(NULL));
}

static void should_return_the_entry_group_correctly(void)
{
    file_entry_set_group(entry_sut, "developers");

    CU_ASSERT_STRING_EQUAL(file_entry_get_group(entry_sut), "developers");
}

static void should_not_set_the_entry_group_if_a_NULL_group_is_passed(void)
{
    file_entry_set_group(entry_sut, "developers");
    file_entry_set_group(entry_sut, NULL);

    CU_ASSERT_STRING_EQUAL(file_entry_get_group(entry_sut), "developers");
}

static void should_not_fail_setting_the_entry_group_if_a_NULL_entry_is_passed(void)
{
    file_entry_set_group(NULL, "developers");
}

static void should_return_zero_size_when_a_NULL_entry_is_passed(void)
{
    CU_ASSERT_EQUAL(file_entry_get_size(NULL), 0);
}

static void should_return_the_entry_size_correctly(void)
{
    file_entry_set_size(entry_sut, 12345);

    CU_ASSERT_EQUAL(file_entry_get_size(entry_sut), 12345);
}

static void should_not_fail_setting_entry_size_if_a_NULL_entry_is_passed(void)
{
    file_entry_set_size(NULL, 12345);
}

static void should_return_an_empty_device_when_a_NULL_entry_is_passed(void)
{
    const t_device device = file_entry_get_device(NULL);

    CU_ASSERT_EQUAL(device.major, 0);
    CU_ASSERT_EQUAL(device.minor, 0);
}

static void should_return_the_entry_device_correctly(void)
{
    const t_device device = { .major = 12, .minor = 34 };

    file_entry_set_device(entry_sut, device);

    const t_device entry_device = file_entry_get_device(entry_sut);
    CU_ASSERT_EQUAL(entry_device.major, 12);
    CU_ASSERT_EQUAL(entry_device.minor, 34);
}

static void should_not_fail_setting_entry_device_if_a_NULL_entry_is_passed(void)
{
    const t_device device = { .major = 12, .minor = 34 };

    file_entry_set_device(NULL, device);
}

static void should_return_an_empty_modification_date_when_a_NULL_entry_is_passed(void)
{
    const t_date modification_date = file_entry_get_modification_date(NULL);

    CU_ASSERT_EQUAL(modification_date.day, 0);
    CU_ASSERT_EQUAL(modification_date.month, 0);
    CU_ASSERT_EQUAL(modification_date.year, 0);
    CU_ASSERT_EQUAL(modification_date.hour, 0);
    CU_ASSERT_EQUAL(modification_date.minute, 0);
    CU_ASSERT_EQUAL(modification_date.second, 0);
    CU_ASSERT_EQUAL(modification_date.timestamp, 0);
}

static void should_return_the_entry_modification_date_correctly(void)
{
    const t_date modification_date = {
        .day = 12,
        .month = 10,
        .year = 2026,
        .hour = 14,
        .minute = 10,
        .second = 42,
        .timestamp = 1791814242
    };

    file_entry_set_modification_date(entry_sut, modification_date);

    const t_date entry_modification_date = file_entry_get_modification_date(entry_sut);
    CU_ASSERT_EQUAL(entry_modification_date.day, modification_date.day);
    CU_ASSERT_EQUAL(entry_modification_date.month, modification_date.month);
    CU_ASSERT_EQUAL(entry_modification_date.year, modification_date.year);
    CU_ASSERT_EQUAL(entry_modification_date.hour, modification_date.hour);
    CU_ASSERT_EQUAL(entry_modification_date.minute, modification_date.minute);
    CU_ASSERT_EQUAL(entry_modification_date.second, modification_date.second);
    CU_ASSERT_EQUAL(entry_modification_date.timestamp, modification_date.timestamp);
}

static void should_not_fail_setting_entry_modification_date_if_a_NULL_entry_is_passed(void)
{
    const t_date modification_date = { .year = 2026 };

    file_entry_set_modification_date(NULL, modification_date);
}

static void should_return_NULL_for_the_entry_link_target_if_a_NULL_entry_is_passed(void)
{
    CU_ASSERT_PTR_NULL(file_entry_get_link_target(NULL));
}

static void should_return_the_entry_link_target_correctly(void)
{
    file_entry_set_file_type(entry_sut, FILE_TYPE_SYMLINK);
    file_entry_set_link_target(entry_sut, "target");

    CU_ASSERT_STRING_EQUAL(file_entry_get_link_target(entry_sut), "target");
}

static void should_not_set_the_entry_link_target_if_the_entry_is_not_a_symlink(void)
{
    file_entry_set_link_target(entry_sut, "target");

    CU_ASSERT_PTR_NULL(file_entry_get_link_target(entry_sut));
}

static void should_not_set_the_entry_link_target_if_a_NULL_target_is_passed(void)
{
    file_entry_set_file_type(entry_sut, FILE_TYPE_SYMLINK);
    file_entry_set_link_target(entry_sut, "target");
    file_entry_set_link_target(entry_sut, NULL);

    CU_ASSERT_STRING_EQUAL(file_entry_get_link_target(entry_sut), "target");
}

static void should_not_set_the_entry_link_target_if_an_empty_target_is_passed(void)
{
    file_entry_set_file_type(entry_sut, FILE_TYPE_SYMLINK);
    file_entry_set_link_target(entry_sut, "target");
    file_entry_set_link_target(entry_sut, "");

    CU_ASSERT_STRING_EQUAL(file_entry_get_link_target(entry_sut), "target");
}

static void should_not_fail_setting_the_entry_link_target_if_a_NULL_entry_is_passed(void)
{
    file_entry_set_link_target(NULL, "target");
}

static void should_return_a_length_of_zero_when_passed_a_NULL_file_entry_array(void)
{
    const unsigned int length = file_entry_array_get_length(NULL);

    CU_ASSERT_EQUAL(length, 0);
}

static void should_return_zero_total_blocks_for_a_NULL_array(void)
{
    CU_ASSERT_EQUAL(file_entry_array_get_total_blocks(NULL), 0);
}

static void should_return_zero_total_blocks_for_an_empty_array(void)
{
    CU_ASSERT_EQUAL(file_entry_array_get_total_blocks(array_sut), 0);
}

static void should_return_the_sum_of_entry_blocks(void)
{
    t_file_entry *first_entry = file_entry_create("first");
    t_file_entry *second_entry = file_entry_create("second");
    t_file_entry *third_entry = file_entry_create("third");

    file_entry_set_blocks(first_entry, 5);
    file_entry_set_blocks(second_entry, 12);
    file_entry_set_blocks(third_entry, 3);
    file_entry_array_push(array_sut, first_entry);
    file_entry_array_push(array_sut, second_entry);
    file_entry_array_push(array_sut, third_entry);

    CU_ASSERT_EQUAL(file_entry_array_get_total_blocks(array_sut), 20);
}

static void should_return_zero_as_the_biggest_link_count_for_a_NULL_array(void)
{
    CU_ASSERT_EQUAL(file_entry_array_get_biggest_link_count(NULL), 0);
}

static void should_return_zero_as_the_biggest_link_count_for_an_empty_array(void)
{
    CU_ASSERT_EQUAL(file_entry_array_get_biggest_link_count(array_sut), 0);
}

static void should_return_biggest_link_count_width(void)
{
    t_file_entry *first_entry = file_entry_create("first");
    t_file_entry *second_entry = file_entry_create("second");
    t_file_entry *third_entry = file_entry_create("third");

    file_entry_set_link_count(first_entry, 3);
    file_entry_set_link_count(second_entry, 10);
    file_entry_set_link_count(third_entry, 5);
    file_entry_array_push(array_sut, first_entry);
    file_entry_array_push(array_sut, second_entry);
    file_entry_array_push(array_sut, third_entry);

    CU_ASSERT_EQUAL(file_entry_array_get_biggest_link_count(array_sut), 2);
}

static void should_return_zero_author_width_for_a_NULL_array(void)
{
    CU_ASSERT_EQUAL(file_entry_array_get_biggest_author_width(NULL), 0);
}

static void should_return_zero_group_width_for_a_NULL_array(void)
{
    CU_ASSERT_EQUAL(file_entry_array_get_biggest_group_width(NULL), 0);
}

static void should_return_zero_author_width_for_an_empty_array(void)
{
    CU_ASSERT_EQUAL(file_entry_array_get_biggest_author_width(array_sut), 0);
}

static void should_return_zero_group_width_for_an_empty_array(void)
{
    CU_ASSERT_EQUAL(file_entry_array_get_biggest_group_width(array_sut), 0);
}

static void should_return_biggest_author_width(void)
{
    t_file_entry *first_entry = file_entry_create("first");
    t_file_entry *second_entry = file_entry_create("second");
    t_file_entry *third_entry = file_entry_create("third");

    file_entry_set_author(first_entry, "alice");
    file_entry_set_author(second_entry, "longer-author");
    file_entry_set_author(third_entry, "bob");
    file_entry_array_push(array_sut, first_entry);
    file_entry_array_push(array_sut, second_entry);
    file_entry_array_push(array_sut, third_entry);

    CU_ASSERT_EQUAL(file_entry_array_get_biggest_author_width(array_sut), 13);
}

static void should_return_biggest_group_width(void)
{
    t_file_entry *first_entry = file_entry_create("first");
    t_file_entry *second_entry = file_entry_create("second");
    t_file_entry *third_entry = file_entry_create("third");

    file_entry_set_group(first_entry, "staff");
    file_entry_set_group(second_entry, "dev");
    file_entry_set_group(third_entry, "longer-group");
    file_entry_array_push(array_sut, first_entry);
    file_entry_array_push(array_sut, second_entry);
    file_entry_array_push(array_sut, third_entry);

    CU_ASSERT_EQUAL(file_entry_array_get_biggest_group_width(array_sut), 12);
}

static void should_return_zero_biggest_size_for_a_NULL_array(void)
{
    CU_ASSERT_EQUAL(file_entry_array_get_biggest_size(NULL), 0);
}

static void should_return_zero_biggest_device_width_for_a_NULL_array(void)
{
    CU_ASSERT_EQUAL(file_entry_array_get_biggest_device_width(NULL), 0);
}

static void should_return_zero_biggest_size_for_an_empty_array(void)
{
    CU_ASSERT_EQUAL(file_entry_array_get_biggest_size(array_sut), 0);
}

static void should_return_zero_biggest_device_width_for_an_empty_array(void)
{
    CU_ASSERT_EQUAL(file_entry_array_get_biggest_device_width(array_sut), 0);
}

static void should_return_biggest_size_width(void)
{
    t_file_entry *first_entry = file_entry_create("first");
    t_file_entry *second_entry = file_entry_create("second");
    t_file_entry *third_entry = file_entry_create("third");

    file_entry_set_size(first_entry, 12);
    file_entry_set_size(second_entry, 1024);
    file_entry_set_size(third_entry, 512);
    file_entry_array_push(array_sut, first_entry);
    file_entry_array_push(array_sut, second_entry);
    file_entry_array_push(array_sut, third_entry);

    CU_ASSERT_EQUAL(file_entry_array_get_biggest_size(array_sut), 4);
}

static void should_return_biggest_device_width(void)
{
    t_file_entry *first_entry = file_entry_create("first");
    t_file_entry *second_entry = file_entry_create("second");

    file_entry_set_file_type(first_entry, FILE_TYPE_CHARDEVICE);
    file_entry_set_device(first_entry, (t_device){ .major = 3, .minor = 4 });
    file_entry_set_file_type(second_entry, FILE_TYPE_BLOCKDEVICE);
    file_entry_set_device(second_entry, (t_device){ .major = 1, .minor = 12 });
    file_entry_array_push(array_sut, first_entry);
    file_entry_array_push(array_sut, second_entry);

    CU_ASSERT_EQUAL(file_entry_array_get_biggest_device_width(array_sut), 3);
}

static void should_return_zero_biggest_date_width_for_a_NULL_array(void)
{
    CU_ASSERT_EQUAL(file_entry_array_get_biggest_date_width(NULL), 0);
}

static void should_return_one_as_biggest_date_width_for_an_empty_array(void)
{
    CU_ASSERT_EQUAL(file_entry_array_get_biggest_date_width(array_sut), 1);
}

static void should_return_one_as_biggest_date_width_when_all_entries_are_unknown(void)
{
    file_entry_array_push(array_sut, file_entry_create("failed"));

    CU_ASSERT_EQUAL(file_entry_array_get_biggest_date_width(array_sut), 1);
}

static void should_return_twelve_as_biggest_date_width_when_a_valid_entry_is_added(void)
{
    t_file_entry *valid_entry = file_entry_create("valid");

    file_entry_set_file_type(valid_entry, FILE_TYPE_REGULAR);
    file_entry_array_push(array_sut, file_entry_create("failed"));
    file_entry_array_push(array_sut, valid_entry);

    CU_ASSERT_EQUAL(file_entry_array_get_biggest_date_width(array_sut), 12);
}

static void should_return_the_correct_length(void)
{
    file_entry_array_push(array_sut, file_entry_create(DEFAULT_NAME));
    file_entry_array_push(array_sut, file_entry_create(DEFAULT_NAME));

    CU_ASSERT_EQUAL(file_entry_array_get_length(array_sut), 2);
}

static void should_return_NULL_for_an_entry_if_a_NULL_array_is_passed(void)
{
    const t_file_entry *entry = file_entry_array_get_at(NULL, 0);

    CU_ASSERT_PTR_NULL(entry);
}

static void should_return_NULL_for_an_entry_if_an_out_of_bounds_index_is_specified(void)
{
    file_entry_array_push(array_sut, file_entry_create("entry 0"));
    file_entry_array_push(array_sut, file_entry_create("entry 1"));
    file_entry_array_push(array_sut, file_entry_create("entry 2"));

    CU_ASSERT_PTR_NULL(file_entry_array_get_at(array_sut, 3));
    CU_ASSERT_PTR_NULL(file_entry_array_get_at(array_sut, 17));
    CU_ASSERT_PTR_NULL(file_entry_array_get_at(array_sut, -1));
}

static void should_return_the_entry_at_the_specified_index(void)
{
    file_entry_array_push(array_sut, file_entry_create("entry 0"));
    file_entry_array_push(array_sut, file_entry_create("entry 1"));
    file_entry_array_push(array_sut, file_entry_create("entry 2"));
    file_entry_array_push(array_sut, file_entry_create("entry 3"));

    const t_file_entry *first_entry = file_entry_array_get_at(array_sut, 0);
    const t_file_entry *second_entry = file_entry_array_get_at(array_sut, 1);
    const t_file_entry *third_entry = file_entry_array_get_at(array_sut, 2);
    const t_file_entry *fourth_entry = file_entry_array_get_at(array_sut, 3);
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
    file_entry_array_push(NULL, entry_sut);
}

static void should_not_fail_pushing_a_NULL_entry_to_an_array(void)
{
    file_entry_array_push(array_sut, NULL);
}

static void should_push_correctly_an_specified_entry(void)
{
    for (unsigned int i = 0; i < 10; i++)
    {
        file_entry_array_push(array_sut, file_entry_create("entry"));
    }

    unsigned int i = 0;
    for (; i < 10; i++)
    {
        CU_ASSERT_PTR_NOT_NULL(file_entry_array_get_at(array_sut, i));
    }
    CU_ASSERT_PTR_NULL(file_entry_array_get_at(array_sut, i));
}

static void should_not_fail_when_sorting_a_NULL_file_entry_array(void)
{
    file_entry_array_sort(NULL);
}

static void should_sort_the_file_operands_by_entry_name(void)
{
    const char *entry_names[] = { "2file", ".hiddir", "_file", "dir", "FILE", "zfile", "zzfile", "zz", NULL };
    file_entry_array_push(array_sut, file_entry_create(entry_names[0]));
    file_entry_array_push(array_sut, file_entry_create(entry_names[1]));
    file_entry_array_push(array_sut, file_entry_create(entry_names[2]));
    file_entry_array_push(array_sut, file_entry_create(entry_names[3]));
    file_entry_array_push(array_sut, file_entry_create(entry_names[4]));
    file_entry_array_push(array_sut, file_entry_create(entry_names[5]));
    file_entry_array_push(array_sut, file_entry_create(entry_names[6]));
    file_entry_array_push(array_sut, file_entry_create(entry_names[7]));

    file_entry_array_sort(array_sut);
    const t_file_entry *first_entry = file_entry_array_get_at(array_sut, 0);
    const t_file_entry *second_entry = file_entry_array_get_at(array_sut, 1);
    const t_file_entry *third_entry = file_entry_array_get_at(array_sut, 2);
    const t_file_entry *fourth_entry = file_entry_array_get_at(array_sut, 3);
    const t_file_entry *fifth_entry = file_entry_array_get_at(array_sut, 4);
    const t_file_entry *sixth_entry = file_entry_array_get_at(array_sut, 5);
    const t_file_entry *seventh_entry = file_entry_array_get_at(array_sut, 6);
    const t_file_entry *eighth_entry = file_entry_array_get_at(array_sut, 7);

    CU_ASSERT_STRING_EQUAL(file_entry_get_name(first_entry), entry_names[1]);
    CU_ASSERT_STRING_EQUAL(file_entry_get_name(second_entry), entry_names[0]);
    CU_ASSERT_STRING_EQUAL(file_entry_get_name(third_entry), entry_names[4]);
    CU_ASSERT_STRING_EQUAL(file_entry_get_name(fourth_entry), entry_names[2]);
    CU_ASSERT_STRING_EQUAL(file_entry_get_name(fifth_entry), entry_names[3]);
    CU_ASSERT_STRING_EQUAL(file_entry_get_name(sixth_entry), entry_names[5]);
    CU_ASSERT_STRING_EQUAL(file_entry_get_name(seventh_entry), entry_names[7]);
    CU_ASSERT_STRING_EQUAL(file_entry_get_name(eighth_entry), entry_names[6]);
    CU_ASSERT_PTR_NULL(file_entry_array_get_at(array_sut, 8));
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
        CU_add_test(suite, "should_return_the_entry_name_correctly", should_return_the_entry_name_correctly);
        CU_add_test(suite, "should_not_fail_setting_the_entry_name_if_a_NULL_file_entry_is_passed", should_not_fail_setting_the_entry_name_if_a_NULL_file_entry_is_passed);
        CU_add_test(suite, "should_not_set_the_entry_name_if_a_NULL_file_name_is_passed", should_not_set_the_entry_name_if_a_NULL_file_name_is_passed);
        CU_add_test(suite, "should_not_set_the_entry_name_if_an_empty_file_name_is_passed", should_not_set_the_entry_name_if_an_empty_file_name_is_passed);
        CU_add_test(suite, "should_return_zero_blocks_when_a_NULL_entry_is_passed", should_return_zero_blocks_when_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_return_the_entry_blocks_correctly", should_return_the_entry_blocks_correctly);
        CU_add_test(suite, "should_not_fail_setting_entry_blocks_if_a_NULL_entry_is_passed", should_not_fail_setting_entry_blocks_if_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_return_no_permissions_when_a_NULL_entry_is_passed", should_return_no_permissions_when_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_return_the_entry_permissions_correctly", should_return_the_entry_permissions_correctly);
        CU_add_test(suite, "should_not_fail_setting_entry_permissions_if_a_NULL_entry_is_passed", should_not_fail_setting_entry_permissions_if_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_return_unknown_for_the_entry_file_type_if_a_NULL_entry_is_passed", should_return_unknown_for_the_entry_file_type_if_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_return_the_entry_file_type_correctly", should_return_the_entry_file_type_correctly);
        CU_add_test(suite, "should_not_fail_setting_the_entry_file_type_if_a_NULL_file_entry_is_passed", should_not_fail_setting_the_entry_file_type_if_a_NULL_file_entry_is_passed);
        CU_add_test(suite, "should_not_set_the_entry_file_type_if_none_file_type_is_passed", should_not_set_the_entry_file_type_if_none_file_type_is_passed);
        CU_add_test(suite, "should_return_zero_link_count_when_a_NULL_entry_is_passed", should_return_zero_link_count_when_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_return_the_entry_link_count_correctly", should_return_the_entry_link_count_correctly);
        CU_add_test(suite, "should_not_fail_setting_entry_link_count_if_a_NULL_entry_is_passed", should_not_fail_setting_entry_link_count_if_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_return_NULL_for_the_entry_author_if_a_NULL_entry_is_passed", should_return_NULL_for_the_entry_author_if_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_return_the_entry_author_correctly", should_return_the_entry_author_correctly);
        CU_add_test(suite, "should_not_set_the_entry_author_if_a_NULL_author_is_passed", should_not_set_the_entry_author_if_a_NULL_author_is_passed);
        CU_add_test(suite, "should_not_fail_setting_the_entry_author_if_a_NULL_entry_is_passed", should_not_fail_setting_the_entry_author_if_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_return_NULL_for_the_entry_group_if_a_NULL_entry_is_passed", should_return_NULL_for_the_entry_group_if_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_return_the_entry_group_correctly", should_return_the_entry_group_correctly);
        CU_add_test(suite, "should_not_set_the_entry_group_if_a_NULL_group_is_passed", should_not_set_the_entry_group_if_a_NULL_group_is_passed);
        CU_add_test(suite, "should_not_fail_setting_the_entry_group_if_a_NULL_entry_is_passed", should_not_fail_setting_the_entry_group_if_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_return_zero_size_when_a_NULL_entry_is_passed", should_return_zero_size_when_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_return_the_entry_size_correctly", should_return_the_entry_size_correctly);
        CU_add_test(suite, "should_not_fail_setting_entry_size_if_a_NULL_entry_is_passed", should_not_fail_setting_entry_size_if_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_return_an_empty_device_when_a_NULL_entry_is_passed", should_return_an_empty_device_when_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_return_the_entry_device_correctly", should_return_the_entry_device_correctly);
        CU_add_test(suite, "should_not_fail_setting_entry_device_if_a_NULL_entry_is_passed", should_not_fail_setting_entry_device_if_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_return_an_empty_modification_date_when_a_NULL_entry_is_passed", should_return_an_empty_modification_date_when_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_return_the_entry_modification_date_correctly", should_return_the_entry_modification_date_correctly);
        CU_add_test(suite, "should_not_fail_setting_entry_modification_date_if_a_NULL_entry_is_passed", should_not_fail_setting_entry_modification_date_if_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_return_NULL_for_the_entry_link_target_if_a_NULL_entry_is_passed", should_return_NULL_for_the_entry_link_target_if_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_return_the_entry_link_target_correctly", should_return_the_entry_link_target_correctly);
        CU_add_test(suite, "should_not_set_the_entry_link_target_if_the_entry_is_not_a_symlink", should_not_set_the_entry_link_target_if_the_entry_is_not_a_symlink);
        CU_add_test(suite, "should_not_set_the_entry_link_target_if_a_NULL_target_is_passed", should_not_set_the_entry_link_target_if_a_NULL_target_is_passed);
        CU_add_test(suite, "should_not_set_the_entry_link_target_if_an_empty_target_is_passed", should_not_set_the_entry_link_target_if_an_empty_target_is_passed);
        CU_add_test(suite, "should_not_fail_setting_the_entry_link_target_if_a_NULL_entry_is_passed", should_not_fail_setting_the_entry_link_target_if_a_NULL_entry_is_passed);
        CU_add_test(suite, "should_return_a_length_of_zero_when_passed_a_NULL_file_entry_array", should_return_a_length_of_zero_when_passed_a_NULL_file_entry_array);
        CU_add_test(suite, "should_return_zero_total_blocks_for_a_NULL_array", should_return_zero_total_blocks_for_a_NULL_array);
        CU_add_test(suite, "should_return_zero_total_blocks_for_an_empty_array", should_return_zero_total_blocks_for_an_empty_array);
        CU_add_test(suite, "should_return_the_sum_of_entry_blocks", should_return_the_sum_of_entry_blocks);
        CU_add_test(suite, "should_return_zero_as_the_biggest_link_count_for_a_NULL_array", should_return_zero_as_the_biggest_link_count_for_a_NULL_array);
        CU_add_test(suite, "should_return_zero_as_the_biggest_link_count_for_an_empty_array", should_return_zero_as_the_biggest_link_count_for_an_empty_array);
        CU_add_test(suite, "should_return_biggest_link_count_width", should_return_biggest_link_count_width);
        CU_add_test(suite, "should_return_zero_author_width_for_a_NULL_array", should_return_zero_author_width_for_a_NULL_array);
        CU_add_test(suite, "should_return_zero_group_width_for_a_NULL_array", should_return_zero_group_width_for_a_NULL_array);
        CU_add_test(suite, "should_return_zero_author_width_for_an_empty_array", should_return_zero_author_width_for_an_empty_array);
        CU_add_test(suite, "should_return_zero_group_width_for_an_empty_array", should_return_zero_group_width_for_an_empty_array);
        CU_add_test(suite, "should_return_biggest_author_width", should_return_biggest_author_width);
        CU_add_test(suite, "should_return_biggest_group_width", should_return_biggest_group_width);
        CU_add_test(suite, "should_return_zero_biggest_size_for_a_NULL_array", should_return_zero_biggest_size_for_a_NULL_array);
        CU_add_test(suite, "should_return_zero_biggest_device_width_for_a_NULL_array", should_return_zero_biggest_device_width_for_a_NULL_array);
        CU_add_test(suite, "should_return_zero_biggest_size_for_an_empty_array", should_return_zero_biggest_size_for_an_empty_array);
        CU_add_test(suite, "should_return_zero_biggest_device_width_for_an_empty_array", should_return_zero_biggest_device_width_for_an_empty_array);
        CU_add_test(suite, "should_return_biggest_size_width", should_return_biggest_size_width);
        CU_add_test(suite, "should_return_biggest_device_width", should_return_biggest_device_width);
        CU_add_test(suite, "should_return_zero_biggest_date_width_for_a_NULL_array", should_return_zero_biggest_date_width_for_a_NULL_array);
        CU_add_test(suite, "should_return_one_as_biggest_date_width_for_an_empty_array", should_return_one_as_biggest_date_width_for_an_empty_array);
        CU_add_test(suite, "should_return_one_as_biggest_date_width_when_all_entries_are_unknown", should_return_one_as_biggest_date_width_when_all_entries_are_unknown);
        CU_add_test(suite, "should_return_twelve_as_biggest_date_width_when_a_valid_entry_is_added", should_return_twelve_as_biggest_date_width_when_a_valid_entry_is_added);
        CU_add_test(suite, "should_return_the_correct_length", should_return_the_correct_length);
        CU_add_test(suite, "should_return_NULL_for_an_entry_if_a_NULL_array_is_passed", should_return_NULL_for_an_entry_if_a_NULL_array_is_passed);
        CU_add_test(suite, "should_return_NULL_for_an_entry_if_an_out_of_bounds_index_is_specified", should_return_NULL_for_an_entry_if_an_out_of_bounds_index_is_specified);
        CU_add_test(suite, "should_return_the_entry_at_the_specified_index", should_return_the_entry_at_the_specified_index);
        CU_add_test(suite, "should_not_fail_pushing_an_entry_to_a_NULL_array", should_not_fail_pushing_an_entry_to_a_NULL_array);
        CU_add_test(suite, "should_not_fail_pushing_a_NULL_entry_to_an_array", should_not_fail_pushing_a_NULL_entry_to_an_array);
        CU_add_test(suite, "should_push_correctly_an_specified_entry", should_push_correctly_an_specified_entry);
        CU_add_test(suite, "should_not_fail_when_sorting_a_NULL_file_entry_array", should_not_fail_when_sorting_a_NULL_file_entry_array);
        CU_add_test(suite, "should_sort_the_file_operands_by_entry_name", should_sort_the_file_operands_by_entry_name);
    }
}
