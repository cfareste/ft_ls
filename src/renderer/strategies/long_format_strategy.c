#include "render_strategies.h"
#include "libft.h"

#define SIX_MONTHS_IN_SECONDS (6 * 30 * 24 * 60 * 60)

static char get_file_type_character(const t_file_type file_type)
{
    if (file_type == FILE_TYPE_REGULAR)
        return '-';
    if (file_type == FILE_TYPE_DIRECTORY)
        return 'd';
    if (file_type == FILE_TYPE_BLOCKDEVICE)
        return 'b';
    if (file_type == FILE_TYPE_CHARDEVICE)
        return 'c';
    if (file_type == FILE_TYPE_SYMLINK)
        return 'l';
    if (file_type == FILE_TYPE_FIFO)
        return 'p';
    if (file_type == FILE_TYPE_SOCKET)
        return 's';

    return '?';
}

static char get_permission_character(const t_permissions file_permissions, const t_permissions permission, const char character)
{
    if (file_permissions & permission)
        return character;

    return '-';
}

static char get_read_permission_character(const t_permissions file_permissions, const t_permissions permission)
{
    return get_permission_character(file_permissions, permission, 'r');
}

static char get_write_permission_character(const t_permissions file_permissions, const t_permissions permission)
{
    return get_permission_character(file_permissions, permission, 'w');
}

static char get_special_execute_character(const t_permissions special_permission, const int execute_is_set)
{
    if (special_permission == PERM_STICKY)
        return execute_is_set ? 't' : 'T';
    return execute_is_set ? 's' : 'S';
}

static char get_special_execute_permission_character(const t_permissions file_permissions, const t_permissions execute_permission, const t_permissions special_permission)
{
    const int execute_is_set = file_permissions & execute_permission;
    const t_permissions displayed_permission = execute_is_set
        ? execute_permission
        : special_permission;
    const char character = get_special_execute_character(special_permission,
                                                         execute_is_set);

    return get_permission_character(file_permissions, displayed_permission, character);
}

static char get_execute_permission_character(const t_permissions file_permissions, const t_permissions execute_permission, const t_permissions special_permission)
{
    if (file_permissions & special_permission)
        return get_special_execute_permission_character(file_permissions,
                                                        execute_permission,
                                                        special_permission);
    return get_permission_character(file_permissions, execute_permission, 'x');
}

static const char *get_month_abbreviation(const int month)
{
    static const char *months[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    if (month < 1 || month > 12)
        return "???";
    return months[month - 1];
}

static int is_recent_modification(const t_date modification_date)
{
    const time_t current_time = date_get_current_time();

    return modification_date.timestamp >= current_time - SIX_MONTHS_IN_SECONDS;
}

static size_t get_unsigned_number_width(unsigned long number)
{
    size_t width = 1;

    while (number >= 10)
    {
        number /= 10;
        width++;
    }
    return width;
}

static void print_padding(const size_t width)
{
    for (size_t i = 0; i < width; i++)
        ft_printf(" ");
}

static void print_mode_string(const t_file_entry *entry)
{
    const t_file_type file_type = file_entry_get_file_type(entry);
    const t_permissions file_permissions = file_entry_get_permissions(entry);

    ft_printf("%c%c%c%c%c%c%c%c%c%c ",
              get_file_type_character(file_type),
              get_read_permission_character(file_permissions, PERM_OWNER_READ),
              get_write_permission_character(file_permissions, PERM_OWNER_WRITE),
              get_execute_permission_character(file_permissions, PERM_OWNER_EXECUTE, PERM_SETUID),
              get_read_permission_character(file_permissions, PERM_GROUP_READ),
              get_write_permission_character(file_permissions, PERM_GROUP_WRITE),
              get_execute_permission_character(file_permissions, PERM_GROUP_EXECUTE, PERM_SETGID),
              get_read_permission_character(file_permissions, PERM_OTHER_READ),
              get_write_permission_character(file_permissions, PERM_OTHER_WRITE),
              get_execute_permission_character(file_permissions, PERM_OTHER_EXECUTE, PERM_STICKY));
}

static void print_link_count(const t_file_entry *entry, const t_file_entry_array *entries)
{
    const unsigned long link_count = file_entry_get_link_count(entry);
    const size_t width = file_entry_array_get_biggest_link_count(entries);

    print_padding(width - get_unsigned_number_width(link_count));
    ft_printf("%lu ", link_count);
}

static void print_author(const t_file_entry *entry, const t_file_entry_array *entries)
{
    const char *author = file_entry_get_author(entry);
    const size_t author_width = author == NULL ? 0 : ft_strlen(author);
    const size_t width = file_entry_array_get_biggest_author_width(entries);

    ft_printf("%s ", author == NULL ? "" : author);
    print_padding(width - author_width);
}

static void print_group(const t_file_entry *entry, const t_file_entry_array *entries)
{
    const char *group = file_entry_get_group(entry);
    const size_t group_width = group == NULL ? 0 : ft_strlen(group);
    const size_t width = file_entry_array_get_biggest_group_width(entries);

    ft_printf("%s ", group == NULL ? "" : group);
    print_padding(width - group_width);
}

static void print_size(const t_file_entry *entry, const t_file_entry_array *entries)
{
    const t_file_type file_type = file_entry_get_file_type(entry);
    const size_t size_width = file_entry_array_get_biggest_size(entries);
    const size_t device_digits = file_entry_array_get_biggest_device_width(entries);
    const size_t device_width = device_digits == 0 ? 0 : device_digits + 2;
    const size_t column_width = size_width > device_width ? size_width : device_width;

    if (file_type == FILE_TYPE_CHARDEVICE || file_type == FILE_TYPE_BLOCKDEVICE)
    {
        const t_device device = file_entry_get_device(entry);
        const size_t entry_device_width = get_unsigned_number_width(device.major)
            + get_unsigned_number_width(device.minor) + 2;

        print_padding(column_width - entry_device_width);
        ft_printf("%u, %u ", device.major, device.minor);
    }
    else
    {
        const long size = file_entry_get_size(entry);
        const size_t entry_size_width = get_unsigned_number_width((unsigned long)size);

        print_padding(column_width - entry_size_width);
        ft_printf("%ld ", size);
    }
}

static void print_modification_date(const t_file_entry *entry)
{
    const t_date modification_date = file_entry_get_modification_date(entry);

    if (is_recent_modification(modification_date))
    {
        ft_printf("%s %2d %02d:%02d ",
                  get_month_abbreviation(modification_date.month),
                  modification_date.day,
                  modification_date.hour,
                  modification_date.minute);
    }
    else
    {
        ft_printf("%s %2d  %d ",
                  get_month_abbreviation(modification_date.month),
                  modification_date.day,
                  modification_date.year);
    }
}

static void print_entry_name(const t_file_entry *entry)
{
    const char *name = file_entry_get_name(entry);
    const char *target = file_entry_get_link_target(entry);

    ft_printf("%s", name);
    if (target != NULL)
        ft_printf(" -> %s", target);
}

static void print_entry(const t_file_entry *entry, const t_file_entry_array *entries)
{
    print_mode_string(entry);
    print_link_count(entry, entries);
    print_author(entry, entries);
    print_group(entry, entries);
    print_size(entry, entries);
    print_modification_date(entry);
    print_entry_name(entry);
    ft_printf("\n");
}

void long_format_strategy(const t_file_entry_array *file_entry_array)
{
    const unsigned int count = file_entry_array_get_length(file_entry_array);

    for (unsigned int i = 0; i < count; i++)
    {
        const t_file_entry *entry = file_entry_array_get_at(file_entry_array, i);

        print_entry(entry, file_entry_array);
    }
}
