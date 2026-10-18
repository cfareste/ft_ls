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

static char get_execute_permission_character(const t_permissions file_permissions,
                                            const t_permissions execute_permission,
                                            const t_permissions special_permission,
                                            const char special_character,
                                            const char special_character_without_execute)
{
    if (file_permissions & special_permission)
    {
        if (file_permissions & execute_permission)
            return special_character;
        return special_character_without_execute;
    }

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

static void print_mode_string(const t_file_entry *entry)
{
    const t_file_type file_type = file_entry_get_file_type(entry);
    const t_permissions file_permissions = file_entry_get_permissions(entry);

    ft_printf("%c%c%c%c%c%c%c%c%c%c ",
                get_file_type_character(file_type),
                get_permission_character(file_permissions, PERM_OWNER_READ, 'r'),
                get_permission_character(file_permissions, PERM_OWNER_WRITE, 'w'),
                get_execute_permission_character(file_permissions, PERM_OWNER_EXECUTE, PERM_SETUID, 's', 'S'),
                get_permission_character(file_permissions, PERM_GROUP_READ, 'r'),
                get_permission_character(file_permissions, PERM_GROUP_WRITE, 'w'),
                get_execute_permission_character(file_permissions, PERM_GROUP_EXECUTE, PERM_SETGID, 's', 'S'),
                get_permission_character(file_permissions, PERM_OTHER_READ, 'r'),
                get_permission_character(file_permissions, PERM_OTHER_WRITE, 'w'),
                get_execute_permission_character(file_permissions, PERM_OTHER_EXECUTE, PERM_STICKY, 't', 'T'));
}

static void print_link_count(const t_file_entry *entry)
{
    ft_printf("%lu ", file_entry_get_link_count(entry));
}

static void print_author(const t_file_entry *entry)
{
    const char *author = file_entry_get_author(entry);

    ft_printf("%s ", author == NULL ? "" : author);
}

static void print_group(const t_file_entry *entry)
{
    const char *group = file_entry_get_group(entry);

    ft_printf("%s ", group == NULL ? "" : group);
}

static void print_size(const t_file_entry *entry)
{
    const t_file_type file_type = file_entry_get_file_type(entry);

    if (file_type == FILE_TYPE_CHARDEVICE || file_type == FILE_TYPE_BLOCKDEVICE)
    {
        const t_device device = file_entry_get_device(entry);
        ft_printf("%u, %u ", device.major, device.minor);
    }
    else
        ft_printf("%ld ", file_entry_get_size(entry));
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

    if (file_entry_get_file_type(entry) == FILE_TYPE_SYMLINK)
    {
        const char *target = file_entry_get_link_target(entry);

        ft_printf("%s -> %s", name, target == NULL ? "" : target);
    }
    else
        ft_printf("%s", name);
}

static void print_entry(const t_file_entry *entry)
{
    print_mode_string(entry);
    print_link_count(entry);
    print_author(entry);
    print_group(entry);
    print_size(entry);
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

        print_entry(entry);
    }
}
