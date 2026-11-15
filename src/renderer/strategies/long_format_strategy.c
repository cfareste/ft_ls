#include "render_strategies.h"
#include "libft.h"

#define SIX_MONTHS_IN_SECONDS (6 * 30 * 24 * 60 * 60)

typedef struct s_permission_column
{
    t_permissions permission;
    t_permissions special_permission;
    char character;
    char special_character;
    char special_without_execute;
} t_permission_column;

typedef struct s_column_widths
{
    size_t link_count;
    size_t author;
    size_t group;
    size_t size;
    size_t date;
} t_column_widths;

static const t_permission_column PERMISSION_COLUMNS[] = {
    { PERM_OWNER_READ, PERM_NONE, 'r', 0, 0 },
    { PERM_OWNER_WRITE, PERM_NONE, 'w', 0, 0 },
    { PERM_OWNER_EXECUTE, PERM_SETUID, 'x', 's', 'S' },
    { PERM_GROUP_READ, PERM_NONE, 'r', 0, 0 },
    { PERM_GROUP_WRITE, PERM_NONE, 'w', 0, 0 },
    { PERM_GROUP_EXECUTE, PERM_SETGID, 'x', 's', 'S' },
    { PERM_OTHER_READ, PERM_NONE, 'r', 0, 0 },
    { PERM_OTHER_WRITE, PERM_NONE, 'w', 0, 0 },
    { PERM_OTHER_EXECUTE, PERM_STICKY, 'x', 't', 'T' }
};

static char get_file_type_character(const t_file_type file_type)
{
    switch (file_type)
    {
        case FILE_TYPE_REGULAR: return '-';
        case FILE_TYPE_DIRECTORY: return 'd';
        case FILE_TYPE_BLOCKDEVICE: return 'b';
        case FILE_TYPE_CHARDEVICE: return 'c';
        case FILE_TYPE_SYMLINK: return 'l';
        case FILE_TYPE_FIFO: return 'p';
        case FILE_TYPE_SOCKET: return 's';
        default: return '?';
    }
}

static char get_permission_character(const t_permissions permissions, const t_permission_column column)
{
    if (column.special_permission != PERM_NONE && permissions & column.special_permission)
    {
        if (permissions & column.permission)
            return column.special_character;
        return column.special_without_execute;
    }

    return permissions & column.permission ? column.character : '-';
}

static const char *get_month_abbreviation(const int month)
{
    static const char *months[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    if (month < 1 || month > 12)
        return "?";
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

static void print_alignment_padding(const size_t column_width, const size_t value_width)
{
    if (column_width > value_width)
        print_padding(column_width - value_width);
}

static t_column_widths get_column_widths(const t_file_entry_array *entries)
{
    const size_t size_width = file_entry_array_get_biggest_size(entries);
    const size_t device_digits = file_entry_array_get_biggest_device_width(entries);
    const size_t device_width = device_digits == 0 ? 0 : device_digits + 2;
    const size_t column_size_width = size_width > device_width ? size_width : device_width;

    return (t_column_widths){
        .link_count = file_entry_array_get_biggest_link_count(entries),
        .author = file_entry_array_get_biggest_author_width(entries),
        .group = file_entry_array_get_biggest_group_width(entries),
        .size = column_size_width,
        .date = file_entry_array_get_biggest_date_width(entries)
    };
}

static void print_mode_string(const t_file_entry *entry)
{
    const t_permissions permissions = file_entry_get_permissions(entry);
    char mode[11];

    mode[0] = get_file_type_character(file_entry_get_file_type(entry));
    for (size_t i = 0; i < sizeof(PERMISSION_COLUMNS) / sizeof(PERMISSION_COLUMNS[0]); i++)
        mode[i + 1] = get_permission_character(permissions, PERMISSION_COLUMNS[i]);
    mode[sizeof(mode) - 1] = '\0';
    ft_printf("%s ", mode);
}

static void print_link_count(const t_file_entry *entry, const t_column_widths widths)
{
    const unsigned long link_count = file_entry_get_link_count(entry);
    const size_t value_width = get_unsigned_number_width(link_count);

    print_alignment_padding(widths.link_count, value_width);
    ft_printf("%lu ", link_count);
}

static void print_padded_text(const char *text, const size_t width)
{
    const char *display_text = text == NULL ? "?" : text;
    const size_t text_width = ft_strlen(display_text);

    ft_printf("%s ", display_text);
    print_alignment_padding(width, text_width);
}

static void print_size(const t_file_entry *entry, const t_column_widths widths)
{
    const t_file_type file_type = file_entry_get_file_type(entry);

    if (file_type == FILE_TYPE_CHARDEVICE || file_type == FILE_TYPE_BLOCKDEVICE)
    {
        const t_device device = file_entry_get_device(entry);
        const size_t device_width = get_unsigned_number_width(device.major)
            + get_unsigned_number_width(device.minor) + 2;

        print_alignment_padding(widths.size, device_width);
        ft_printf("%u, %u ", device.major, device.minor);
        return;
    }

    const long size = file_entry_get_size(entry);
    const size_t size_width = get_unsigned_number_width((unsigned long)size);

    print_alignment_padding(widths.size, size_width);
    ft_printf("%ld ", size);
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
        return;
    }

    ft_printf("%s %2d  %d ",
              get_month_abbreviation(modification_date.month),
              modification_date.day,
              modification_date.year);
}

static void print_failed_entry(const t_file_entry *entry, const t_column_widths widths)
{
    print_mode_string(entry);
    print_alignment_padding(widths.link_count, 1);
    ft_printf("? ");
    print_padded_text(NULL, widths.author);
    print_padded_text(NULL, widths.group);
    print_alignment_padding(widths.size, 1);
    ft_printf("? ?");
    print_alignment_padding(widths.date, 1);
    ft_printf(" %s\n", file_entry_get_name(entry));
}

static void print_entry_name(const t_file_entry *entry)
{
    const char *name = file_entry_get_name(entry);
    const char *target = file_entry_get_link_target(entry);

    ft_printf("%s", name);
    if (target != NULL)
        ft_printf(" -> %s", target);
}

static void print_entry(const t_file_entry *entry, const t_column_widths widths)
{
    if (file_entry_get_file_type(entry) == FILE_TYPE_UNKNOWN)
    {
        print_failed_entry(entry, widths);
        return;
    }

    print_mode_string(entry);
    print_link_count(entry, widths);
    print_padded_text(file_entry_get_author(entry), widths.author);
    print_padded_text(file_entry_get_group(entry), widths.group);
    print_size(entry, widths);
    print_modification_date(entry);
    print_entry_name(entry);
    ft_printf("\n");
}

void directory_entries_prefix_long_format_strategy(const t_file_entry_array *file_entry_array)
{
    const unsigned long total_blocks = (file_entry_array_get_total_blocks(file_entry_array) + 1) / 2;

    ft_printf("total %lu\n", total_blocks);
}

void long_format_strategy(const t_file_entry_array *file_entry_array)
{
    const unsigned int count = file_entry_array_get_length(file_entry_array);
    const t_column_widths widths = get_column_widths(file_entry_array);

    for (unsigned int i = 0; i < count; i++)
        print_entry(file_entry_array_get_at(file_entry_array, i), widths);
}
