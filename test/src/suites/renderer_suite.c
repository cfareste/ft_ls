#include "suites.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "renderer.h"
#include "mocks.h"

#define SUITE_NAME "renderer"

static t_result *parsed_arguments_result;
static t_parsed_arguments *parsed_arguments;

static void test_setup(void)
{
    reset_printing_buffer();
    vfs_mock_reset();
}

static void test_teardown(void)
{
    result_destroy(&parsed_arguments_result);
    parsed_arguments_destroy(&parsed_arguments);
}

static void get_parsed_arguments_result(const int argc, const char **args)
{
    parsed_arguments_result = parse_arguments(argc, args);
    parsed_arguments = result_get_value(parsed_arguments_result);
}

static t_file_entry *create_entry(const char *name, const t_file_type file_type,
                                              const t_permissions permissions, const unsigned long link_count,
                                              const char *author, const char *group, const long size,
                                              const t_date modification_date)
{
    t_file_entry *entry = file_entry_create(name);

    file_entry_set_file_type(entry, file_type);
    file_entry_set_permissions(entry, permissions);
    file_entry_set_link_count(entry, link_count);
    file_entry_set_author(entry, author);
    file_entry_set_group(entry, group);
    file_entry_set_size(entry, size);
    file_entry_set_modification_date(entry, modification_date);

    return entry;
}

static void should_create_a_context(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { NULL };
    get_parsed_arguments_result(0, args);
    t_render_context *context = render_context_create(parsed_arguments);

    CU_ASSERT_PTR_NOT_NULL(context);

    render_context_destroy(&context);
}

static void should_return_NULL_when_creating_a_context_if_null_parsed_args_are_specified(void)
{
    t_render_context *context = render_context_create(NULL);

    CU_ASSERT_PTR_NULL(context);

    render_context_destroy(&context);
}

static void should_destroy_a_context(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { NULL };
    get_parsed_arguments_result(0, args);
    t_render_context *context = render_context_create(parsed_arguments);

    render_context_destroy(&context);

    CU_ASSERT_PTR_NULL(context);
}

static void should_not_fail_to_destroy_a_context_when_a_null_pointer_is_passed(void)
{
    render_context_destroy(NULL);
}

static void should_not_fail_to_destroy_a_context_when_an_already_null_context_is_passed(void)
{
    t_render_context *invalid = NULL;

    render_context_destroy(&invalid);
}

static void should_not_render_anything_if_the_context_is_null(void)
{
    t_file_entry_array *file_entry_array = file_entry_array_create();
    t_file_entry *file_entry = file_entry_create("valid");
    file_entry_array_push(file_entry_array, file_entry);

    render_entries(NULL, file_entry_array);

    CU_ASSERT(verify_that_no_output_was_printed());

    file_entry_array_destroy(&file_entry_array);
}

static void should_not_render_anything_if_the_file_entry_array_is_null(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR(".", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { NULL };
    get_parsed_arguments_result(0, args);
    t_render_context *context = render_context_create(parsed_arguments);

    render_entries(context, NULL);

    CU_ASSERT(verify_that_no_output_was_printed());

    render_context_destroy(&context);
}

static void should_render_the_name_of_the_entry_with_a_file_entry_array_of_one_element(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *expected_file_name = "file";
    const char *args[] = { expected_file_name, NULL };
    get_parsed_arguments_result(1, args);
    t_file_entry_array *file_entry_array = file_entry_array_create();
    t_file_entry *file_entry = file_entry_create(expected_file_name);
    file_entry_array_push(file_entry_array, file_entry);
    t_render_context *context = render_context_create(parsed_arguments);

    render_entries(context, file_entry_array);

    CU_ASSERT(verify_that_the_output_printed_is("%s\n", expected_file_name));

    file_entry_array_destroy(&file_entry_array);
    parsed_arguments_destroy(&parsed_arguments);
    render_context_destroy(&context);
}

static void should_render_the_name_of_every_entry_with_a_file_entry_array_of_various_elements_separated_by_new_lines(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_FILE("file3"),
        MOCK_FILE("file4"),
        MOCK_FILE("file5"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "file", "file2", "file3", "file4", "file5", NULL };
    get_parsed_arguments_result(5, args);
    t_render_context *context = render_context_create(parsed_arguments);
    const char *expected_file_name[] = { "file", "file2", "file3", "file4", "file5" };
    t_file_entry_array *file_entry_array = file_entry_array_create();
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[0]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[1]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[2]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[3]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[4]));

    render_entries(context, file_entry_array);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n%s\n%s\n%s\n",
        expected_file_name[0],
        expected_file_name[1],
        expected_file_name[2],
        expected_file_name[3],
        expected_file_name[4]
    ));

    file_entry_array_destroy(&file_entry_array);
    parsed_arguments_destroy(&parsed_arguments);
    render_context_destroy(&context);
}

static void should_not_render_a_directory_if_a_NULL_context_is_provided(void)
{
    t_file_entry_array *file_entry_array = file_entry_array_create();

    render_directory(NULL, "valid", file_entry_array);

    CU_ASSERT(verify_that_no_output_was_printed());

    file_entry_array_destroy(&file_entry_array);
}

static void should_not_render_a_directory_if_a_NULL_directory_header_is_provided(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", ".."),
        MOCK_DIR("dir2", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "dir", "dir2", NULL };
    get_parsed_arguments_result(2, args);
    t_render_context *context = render_context_create(parsed_arguments);
    t_file_entry_array *file_entry_array = file_entry_array_create();
    file_entry_array_push(file_entry_array, file_entry_create("file"));

    render_directory(context, NULL, file_entry_array);

    CU_ASSERT(verify_that_no_output_was_printed());

    file_entry_array_destroy(&file_entry_array);
    parsed_arguments_destroy(&parsed_arguments);
    render_context_destroy(&context);
}

static void should_not_render_a_directory_if_an_empty_directory_header_is_provided(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", ".."),
        MOCK_DIR("dir2", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "dir", "dir2", NULL };
    get_parsed_arguments_result(2, args);
    t_render_context *context = render_context_create(parsed_arguments);
    t_file_entry_array *file_entry_array = file_entry_array_create();
    file_entry_array_push(file_entry_array, file_entry_create("file"));

    render_directory(context, "", file_entry_array);

    CU_ASSERT(verify_that_no_output_was_printed());

    file_entry_array_destroy(&file_entry_array);
    parsed_arguments_destroy(&parsed_arguments);
    render_context_destroy(&context);
}

static void should_not_render_a_directory_if_a_NULL_file_entry_array_is_provided(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", ".."),
        MOCK_DIR("dir2", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "dir", "dir2", NULL };
    get_parsed_arguments_result(2, args);
    t_render_context *context = render_context_create(parsed_arguments);

    render_directory(context, "valid", NULL);

    CU_ASSERT(verify_that_no_output_was_printed());

    render_context_destroy(&context);
    parsed_arguments_destroy(&parsed_arguments);
}

static void should_not_render_a_dir_header_if_its_not_needed(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *expected_file_name[] = { "file", "file2", "file3" };
    const char *dir_header = "dir";
    const char *args[] = { "dir", NULL };
    get_parsed_arguments_result(1, args);
    t_render_context *context = render_context_create(parsed_arguments);
    t_file_entry_array *file_entry_array = file_entry_array_create();
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[0]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[1]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[2]));

    render_directory(context, dir_header, file_entry_array);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n%s\n",
        expected_file_name[0],
        expected_file_name[1],
        expected_file_name[2]
    ));

    file_entry_array_destroy(&file_entry_array);
    parsed_arguments_destroy(&parsed_arguments);
    render_context_destroy(&context);
}

static void should_not_render_a_leading_dir_header_newline_if_its_the_first_render(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", ".."),
        MOCK_DIR("dir2", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *expected_file_name[] = { "file", "file2", "file3" };
    const char *dir_header = "dir";
    const char *args[] = { "dir", "dir2", NULL };
    get_parsed_arguments_result(2, args);
    t_render_context *context = render_context_create(parsed_arguments);
    t_file_entry_array *file_entry_array = file_entry_array_create();
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[0]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[1]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[2]));

    render_directory(context, dir_header, file_entry_array);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s:\n%s\n%s\n%s\n",
        dir_header,
        expected_file_name[0],
        expected_file_name[1],
        expected_file_name[2]
    ));

    file_entry_array_destroy(&file_entry_array);
    parsed_arguments_destroy(&parsed_arguments);
    render_context_destroy(&context);
}

static void should_render_a_leading_dir_header_newline_if_its_not_first_render(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", ".."),
        MOCK_DIR("dir2", ".", ".."),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *expected_file_name[] = { "file", "file2" };
    const char *dir_header = "dir";
    const char *args[] = { "dir", "dir2", NULL };
    get_parsed_arguments_result(2, args);
    t_render_context *context = render_context_create(parsed_arguments);
    t_file_entry_array *file_entry_array = file_entry_array_create();
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[0]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[1]));

    render_directory(context, dir_header, file_entry_array);
    render_directory(context, dir_header, file_entry_array);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s:\n"
        "%s\n%s\n"
        "\n%s:\n"
        "%s\n%s\n",
        dir_header,
        expected_file_name[0],
        expected_file_name[1],
        dir_header,
        expected_file_name[0],
        expected_file_name[1]
    ));

    file_entry_array_destroy(&file_entry_array);
    parsed_arguments_destroy(&parsed_arguments);
    render_context_destroy(&context);
}

static void should_render_a_dir_header_if_the_recursive_option_was_specified(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", "..", "file", "file2"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *expected_file_name[] = { "file", "file2" };
    const char *dir_header = "dir";
    const char *args[] = { "dir" , "-R", NULL };
    get_parsed_arguments_result(2, args);
    t_render_context *context = render_context_create(parsed_arguments);
    t_file_entry_array *file_entry_array = file_entry_array_create();
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[0]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[1]));

    render_directory(context, dir_header, file_entry_array);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s:\n%s\n%s\n",
        dir_header,
        expected_file_name[0],
        expected_file_name[1]
    ));

    file_entry_array_destroy(&file_entry_array);
    parsed_arguments_destroy(&parsed_arguments);
    render_context_destroy(&context);
}

static void should_not_render_the_types_separator_if_NULL_context_is_passed(void)
{
    render_types_separator(NULL);

    CU_ASSERT(verify_that_no_output_was_printed());
}

static void should_render_the_types_separator_if_has_at_least_one_non_dir_and_one_dir(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_DIR("dir", ".", ".."),
        MOCK_FILE("file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "dir", "file", NULL };
    get_parsed_arguments_result(2, args);
    t_render_context *context = render_context_create(parsed_arguments);

    render_types_separator(context);

    CU_ASSERT(verify_that_the_output_printed_is("\n"));

    parsed_arguments_destroy(&parsed_arguments);
    render_context_destroy(&context);
}

static void should_not_render_the_types_separator_if_doesnt_have_at_least_one_non_dir_and_one_dir(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "file2", "file", NULL };
    get_parsed_arguments_result(2, args);
    t_render_context *context = render_context_create(parsed_arguments);

    render_types_separator(context);

    CU_ASSERT(verify_that_no_output_was_printed());

    parsed_arguments_destroy(&parsed_arguments);
    render_context_destroy(&context);
}

static void should_render_one_entry_with_long_format(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE_WITH_STATS("file",
            MOCK_HARDLINK_COUNT(2),
            MOCK_AUTHOR("bob"),
            MOCK_GROUP("developers"),
            MOCK_SIZE(1024),
            MOCK_MODIFICATION_TIME(12, 12, 2025, 12, 30, 00)
        ),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "-l", "file", NULL };
    get_parsed_arguments_result(2, args);
    t_file_entry_array *file_entry_array = file_entry_array_create();
    t_file_entry *entry = file_entry_create("file");
    file_entry_set_file_type(entry, FILE_TYPE_REGULAR);
    file_entry_set_permissions(entry, PERM_OWNER_READ | PERM_OWNER_WRITE | PERM_GROUP_READ | PERM_OTHER_READ);
    file_entry_set_link_count(entry, 2);
    file_entry_set_author(entry, "bob");
    file_entry_set_group(entry, "developers");
    file_entry_set_size(entry, 1024);
    file_entry_set_modification_date(entry, (t_date){
        .day = 12, .month = 12, .year = 2100, .hour = 12, .minute = 30, .timestamp = 4102444800
    });
    file_entry_array_push(file_entry_array, entry);
    t_render_context *context = render_context_create(parsed_arguments);

    render_entries(context, file_entry_array);

    CU_ASSERT(verify_that_the_output_printed_is("%s\n", "-rw-r--r-- 2 bob developers 1024 Dec 12 12:30 file"));

    file_entry_array_destroy(&file_entry_array);
    parsed_arguments_destroy(&parsed_arguments);
    render_context_destroy(&context);
}

static void should_render_an_old_modification_file_in_long_format(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "-l", "file", NULL };
    get_parsed_arguments_result(2, args);

    t_file_entry_array *file_entry_array = file_entry_array_create();
    file_entry_array_push(file_entry_array, create_entry("file", FILE_TYPE_REGULAR,
                                                         PERM_OWNER_READ | PERM_OWNER_WRITE | PERM_GROUP_READ | PERM_OTHER_READ,
                                                         2, "bob", "developers", 1024,
                                                         (t_date){ .day = 12, .month = 12, .year = 2025,
                                                                   .hour = 12, .minute = 30, .timestamp = 1 }));
    t_render_context *context = render_context_create(parsed_arguments);

    render_entries(context, file_entry_array);

    CU_ASSERT(verify_that_the_output_printed_is("%s\n", "-rw-r--r-- 2 bob developers 1024 Dec 12  2025 file"));

    file_entry_array_destroy(&file_entry_array);
    render_context_destroy(&context);
    parsed_arguments_destroy(&parsed_arguments);
}

static void should_render_entries_with_setuid_in_long_format(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "-l", "file", NULL };
    get_parsed_arguments_result(2, args);

    const t_permissions permissions = PERM_SETUID | PERM_OWNER_EXECUTE;
    t_file_entry_array *file_entry_array = file_entry_array_create();
    file_entry_array_push(file_entry_array, create_entry("file", FILE_TYPE_REGULAR, permissions, 1,
                                                         "user", "group", 1,
                                                         (t_date){ .day = 1, .month = 1, .year = 2100, .timestamp = 4102444800 }));
    file_entry_array_push(file_entry_array, create_entry("file2", FILE_TYPE_REGULAR, PERM_SETUID, 1,
                                                         "user", "group", 1,
                                                         (t_date){ .day = 1, .month = 1, .year = 2100, .timestamp = 4102444800 }));
    t_render_context *context = render_context_create(parsed_arguments);

    render_entries(context, file_entry_array);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n",
        "---s------ 1 user group 1 Jan  1 00:00 file",
        "---S------ 1 user group 1 Jan  1 00:00 file2"
    ));

    file_entry_array_destroy(&file_entry_array);
    render_context_destroy(&context);
    parsed_arguments_destroy(&parsed_arguments);
}

static void should_render_entries_with_setgid_in_long_format(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "-l", "file", NULL };
    get_parsed_arguments_result(2, args);

    const t_permissions permissions = PERM_SETGID | PERM_GROUP_EXECUTE;
    t_file_entry_array *file_entry_array = file_entry_array_create();
    file_entry_array_push(file_entry_array, create_entry("file", FILE_TYPE_REGULAR, permissions, 1,
                                                         "user", "group", 1,
                                                         (t_date){ .day = 1, .month = 1, .year = 2100, .timestamp = 4102444800 }));
    file_entry_array_push(file_entry_array, create_entry("file2", FILE_TYPE_REGULAR, PERM_SETGID, 1,
                                                         "user", "group", 1,
                                                         (t_date){ .day = 1, .month = 1, .year = 2100, .timestamp = 4102444800 }));
    t_render_context *context = render_context_create(parsed_arguments);

    render_entries(context, file_entry_array);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n",
        "------s--- 1 user group 1 Jan  1 00:00 file",
        "------S--- 1 user group 1 Jan  1 00:00 file2"
    ));

    file_entry_array_destroy(&file_entry_array);
    render_context_destroy(&context);
    parsed_arguments_destroy(&parsed_arguments);
}

static void should_render_entries_with_sticky_bit_in_long_format(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_FILE("file"),
        MOCK_FILE("file2"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "-l", "file", NULL };
    get_parsed_arguments_result(2, args);

    const t_permissions permissions = PERM_STICKY | PERM_OTHER_EXECUTE;
    t_file_entry_array *file_entry_array = file_entry_array_create();
    file_entry_array_push(file_entry_array, create_entry("file", FILE_TYPE_REGULAR, permissions, 1,
                                                         "user", "group", 1,
                                                         (t_date){ .day = 1, .month = 1, .year = 2100, .timestamp = 4102444800 }));
    file_entry_array_push(file_entry_array, create_entry("file2", FILE_TYPE_REGULAR, PERM_STICKY, 1,
                                                         "user", "group", 1,
                                                         (t_date){ .day = 1, .month = 1, .year = 2100, .timestamp = 4102444800 }));
    t_render_context *context = render_context_create(parsed_arguments);

    render_entries(context, file_entry_array);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n",
        "---------t 1 user group 1 Jan  1 00:00 file",
        "---------T 1 user group 1 Jan  1 00:00 file2"
    ));

    file_entry_array_destroy(&file_entry_array);
    render_context_destroy(&context);
    parsed_arguments_destroy(&parsed_arguments);
}

static void should_render_char_and_block_devices_in_long_format(void)
{
    const t_vfs_mock_entry vfs[] = {
        MOCK_CHAR_DEVICE("char-device"),
        MOCK_BLOCK_DEVICE("block-device"),
        MOCK_NULL_TERMINATOR()
    };
    vfs_mock_setup(vfs);

    const char *args[] = { "-l", "char-device", "block-device", NULL };
    get_parsed_arguments_result(3, args);
    t_file_entry_array *file_entry_array = file_entry_array_create();
    t_file_entry *char_device_entry = create_entry("char-device", FILE_TYPE_CHARDEVICE, PERM_NONE, 1,
                                                   "user", "group", 1024,
                                                   (t_date){ .day = 1, .month = 1, .year = 2100, .timestamp = 4102444800 });
    t_file_entry *block_device_entry = create_entry("block-device", FILE_TYPE_BLOCKDEVICE, PERM_NONE, 1,
                                                    "user", "group", 2048,
                                                    (t_date){ .day = 1, .month = 1, .year = 2100, .timestamp = 4102444800 });
    file_entry_set_device(char_device_entry, (t_device){ .major = 12, .minor = 34 });
    file_entry_set_device(block_device_entry, (t_device){ .major = 56, .minor = 78 });
    file_entry_array_push(file_entry_array, char_device_entry);
    file_entry_array_push(file_entry_array, block_device_entry);
    t_render_context *context = render_context_create(parsed_arguments);

    render_entries(context, file_entry_array);

    CU_ASSERT(verify_that_the_output_printed_is(
        "%s\n%s\n",
        "c--------- 1 user group 12, 34 Jan  1 00:00 char-device",
        "b--------- 1 user group 56, 78 Jan  1 00:00 block-device"
    ));

    file_entry_array_destroy(&file_entry_array);
    render_context_destroy(&context);
    parsed_arguments_destroy(&parsed_arguments);
}

/*static void should_render_the_name_of_every_entry_with_a_file_entry_array_of_various_elements_separated_by_two_spaces(void)
{
    const char *expected_file_name[] = { "file", "file2", "file3", "file4", "file5" };
    t_file_entry_array *file_entry_array = file_entry_array_create(expected_file_name[0]);
    file_entry_array_push(&file_entry_array, file_entry_array_create(expected_file_name[1]));
    file_entry_array_push(&file_entry_array, file_entry_array_create(expected_file_name[2]));
    file_entry_array_push(&file_entry_array, file_entry_array_create(expected_file_name[3]));
    file_entry_array_push(&file_entry_array, file_entry_array_create(expected_file_name[4]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[0]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[1]));
    file_entry_array_push(file_entry_array, file_entry_create(expected_file_name[2]));

    render_entries(file_entry_array);

    verify_that_the_output_printed_is(
        "%s  %s  %s  %s  %s\n",
        expected_file_name[0],
        expected_file_name[1],
        expected_file_name[2],
        expected_file_name[3],
        expected_file_name[4]
    );

    file_entry_array_destroy(&file_entry_array);
}*/

void register_renderer_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, test_setup, test_teardown);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_create_a_context", should_create_a_context);
        CU_add_test(suite, "should_return_NULL_when_creating_a_context_if_null_parsed_args_are_specified", should_return_NULL_when_creating_a_context_if_null_parsed_args_are_specified);
        CU_add_test(suite, "should_destroy_a_context", should_destroy_a_context);
        CU_add_test(suite, "should_not_fail_to_destroy_a_context_when_a_null_pointer_is_passed", should_not_fail_to_destroy_a_context_when_a_null_pointer_is_passed);
        CU_add_test(suite, "should_not_fail_to_destroy_a_context_when_an_already_null_context_is_passed", should_not_fail_to_destroy_a_context_when_an_already_null_context_is_passed);
        CU_add_test(suite, "should_not_render_anything_if_the_context_is_null", should_not_render_anything_if_the_context_is_null);
        CU_add_test(suite, "should_not_render_anything_if_the_file_entry_array_is_null", should_not_render_anything_if_the_file_entry_array_is_null);
        CU_add_test(suite, "should_render_the_name_of_the_entry_with_a_file_entry_array_of_one_element", should_render_the_name_of_the_entry_with_a_file_entry_array_of_one_element);
        CU_add_test(suite, "should_render_the_name_of_every_entry_with_a_file_entry_array_of_various_elements_separated_by_new_lines", should_render_the_name_of_every_entry_with_a_file_entry_array_of_various_elements_separated_by_new_lines);
        CU_add_test(suite, "should_not_render_a_directory_if_a_NULL_context_is_provided", should_not_render_a_directory_if_a_NULL_context_is_provided);
        CU_add_test(suite, "should_not_render_a_directory_if_a_NULL_directory_header_is_provided", should_not_render_a_directory_if_a_NULL_directory_header_is_provided);
        CU_add_test(suite, "should_not_render_a_directory_if_an_empty_directory_header_is_provided", should_not_render_a_directory_if_an_empty_directory_header_is_provided);
        CU_add_test(suite, "should_not_render_a_directory_if_a_NULL_file_entry_array_is_provided", should_not_render_a_directory_if_a_NULL_file_entry_array_is_provided);
        CU_add_test(suite, "should_not_render_a_dir_header_if_its_not_needed", should_not_render_a_dir_header_if_its_not_needed);
        CU_add_test(suite, "should_not_render_a_leading_dir_header_newline_if_its_the_first_render", should_not_render_a_leading_dir_header_newline_if_its_the_first_render);
        CU_add_test(suite, "should_render_a_leading_dir_header_newline_if_its_not_first_render", should_render_a_leading_dir_header_newline_if_its_not_first_render);
        CU_add_test(suite, "should_render_a_dir_header_if_the_recursive_option_was_specified", should_render_a_dir_header_if_the_recursive_option_was_specified);
        CU_add_test(suite, "should_not_render_the_types_separator_if_NULL_context_is_passed", should_not_render_the_types_separator_if_NULL_context_is_passed);
        CU_add_test(suite, "should_render_the_types_separator_if_has_at_least_one_non_dir_and_one_dir", should_render_the_types_separator_if_has_at_least_one_non_dir_and_one_dir);
        CU_add_test(suite, "should_not_render_the_types_separator_if_doesnt_have_at_least_one_non_dir_and_one_dir", should_not_render_the_types_separator_if_doesnt_have_at_least_one_non_dir_and_one_dir);
        CU_add_test(suite, "should_render_one_entry_with_long_format", should_render_one_entry_with_long_format);
        CU_add_test(suite, "should_render_entries_with_setuid_in_long_format", should_render_entries_with_setuid_in_long_format);
        CU_add_test(suite, "should_render_entries_with_setgid_in_long_format", should_render_entries_with_setgid_in_long_format);
        CU_add_test(suite, "should_render_entries_with_sticky_bit_in_long_format", should_render_entries_with_sticky_bit_in_long_format);
        CU_add_test(suite, "should_render_char_and_block_devices_in_long_format", should_render_char_and_block_devices_in_long_format);
        CU_add_test(suite, "should_render_an_old_modification_file_in_long_format", should_render_an_old_modification_file_in_long_format);
        // CU_add_test(suite, "should_render_the_name_of_every_entry_with_a_file_entry_array_of_various_elements_separated_by_two_spaces", should_render_the_name_of_every_entry_with_a_file_entry_array_of_various_elements_separated_by_two_spaces);
    }
}
