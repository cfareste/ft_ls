#include "render_colors.h"
#include "libft.h"

#define COLOR_NORMAL ""
#define COLOR_DIRECTORY "\033[01;34m"
#define COLOR_SYMLINK "\033[01;36m"
#define COLOR_FIFO "\033[40;33m"
#define COLOR_SOCKET "\033[01;35m"
#define COLOR_DEVICE "\033[40;33;01m"
#define COLOR_ORPHAN "\033[40;31;01m"
#define COLOR_SETUID "\033[37;41m"
#define COLOR_SETGID "\033[30;43m"
#define COLOR_STICKY_OTHER_WRITABLE "\033[30;42m"
#define COLOR_OTHER_WRITABLE "\033[34;42m"
#define COLOR_STICKY "\033[37;44m"
#define COLOR_EXECUTABLE "\033[01;32m"
#define COLOR_ARCHIVE "\033[01;31m"
#define COLOR_IMAGE "\033[01;35m"
#define COLOR_AUDIO "\033[00;36m"
#define COLOR_TEMPORARY "\033[00;90m"
#define COLOR_RESET "\033[0m"

static int has_permission(const t_permissions permissions, const t_permissions permission)
{
    return (permissions & permission) != 0;
}

static int name_has_suffix(const char *name, const char *suffix)
{
    const size_t name_length = ft_strlen(name);
    const size_t suffix_length = ft_strlen(suffix);

    return name_length >= suffix_length
        && ft_strncmp(name + name_length - suffix_length, suffix, suffix_length) == 0;
}

static int matches_any_suffix(const char *name, const char *const *suffixes, const size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        if (name_has_suffix(name, suffixes[i]))
            return 1;
    }
    return 0;
}

static const char *get_extension_color(const char *name)
{
    static const char *const archives[] = { ".zip", ".tar", ".gz", ".rar", ".7z" };
    static const char *const images[] = { ".jpg", ".jpeg", ".png", ".gif", ".mp4", ".ppm" };
    static const char *const audio[] = { ".mp3", ".wav", ".ogg", ".flac", ".aac" };
    static const char *const temporary[] = { ".tmp", ".bak", ".swp", ".old" };

    if (matches_any_suffix(name, archives, sizeof(archives) / sizeof(archives[0])))
        return COLOR_ARCHIVE;
    if (matches_any_suffix(name, images, sizeof(images) / sizeof(images[0])))
        return COLOR_IMAGE;
    if (matches_any_suffix(name, audio, sizeof(audio) / sizeof(audio[0])))
        return COLOR_AUDIO;
    if (matches_any_suffix(name, temporary, sizeof(temporary) / sizeof(temporary[0]))
        || name_has_suffix(name, "~"))
        return COLOR_TEMPORARY;
    return NULL;
}

static const char *get_regular_file_color(
    const char *name,
    const t_permissions permissions)
{
    if (has_permission(permissions, PERM_SETUID))
        return COLOR_SETUID;
    if (has_permission(permissions, PERM_SETGID))
        return COLOR_SETGID;
    if (has_permission(permissions, PERM_OWNER_EXECUTE)
        || has_permission(permissions, PERM_GROUP_EXECUTE)
        || has_permission(permissions, PERM_OTHER_EXECUTE))
        return COLOR_EXECUTABLE;
    const char *extension_color = get_extension_color(name);
    if (extension_color != NULL)
        return extension_color;
    return COLOR_NORMAL;
}

static const char *get_file_color(
    const char *name,
    const t_file_type file_type,
    const t_permissions permissions)
{
    switch (file_type)
    {
        case FILE_TYPE_DIRECTORY:
            if (has_permission(permissions, PERM_STICKY)
                && has_permission(permissions, PERM_OTHER_WRITE))
                return COLOR_STICKY_OTHER_WRITABLE;
            if (has_permission(permissions, PERM_OTHER_WRITE))
                return COLOR_OTHER_WRITABLE;
            if (has_permission(permissions, PERM_STICKY))
                return COLOR_STICKY;
            return COLOR_DIRECTORY;
        case FILE_TYPE_REGULAR:
            return get_regular_file_color(name, permissions);
        case FILE_TYPE_CHARDEVICE: return COLOR_DEVICE;
        case FILE_TYPE_BLOCKDEVICE: return COLOR_DEVICE;
        case FILE_TYPE_FIFO: return COLOR_FIFO;
        case FILE_TYPE_SYMLINK: return COLOR_SYMLINK;
        case FILE_TYPE_SOCKET: return COLOR_SOCKET;
        default: return COLOR_NORMAL;
    }
}

static const char *get_stats_color(const char *name, const t_file_stats *stats)
{
    return get_file_color(
        name,
        file_stats_get_file_type(stats),
        file_stats_get_permissions(stats));
}

static void print_colored_text(const char *text, const char *color)
{
    if (color == NULL || color[0] == '\0')
    {
        ft_printf("%s", text);
        return;
    }
    ft_printf("%s%s%s", color, text, COLOR_RESET);
}

static int color_is_enabled(const t_parsed_arguments *parsed_arguments)
{
    return parsed_arguments_is_option_active(parsed_arguments, OPTIONS_COLOR);
}

static t_file_stats *get_link_target_stats(const t_file_entry *entry)
{
    return file_stats_get_following_symlinks(file_entry_get_path(entry));
}

static const char *get_entry_color(const t_file_entry *entry, t_file_stats **target_stats)
{
    if (file_entry_get_file_type(entry) != FILE_TYPE_SYMLINK)
        return get_file_color(
            file_entry_get_name(entry),
            file_entry_get_file_type(entry),
            file_entry_get_permissions(entry));

    *target_stats = get_link_target_stats(entry);
    if (*target_stats == NULL)
        return COLOR_ORPHAN;
    return COLOR_SYMLINK;
}

void render_colors_print_name(const t_parsed_arguments *parsed_arguments, const t_file_entry *entry)
{
    if (entry == NULL)
        return;
    if (!color_is_enabled(parsed_arguments))
    {
        ft_printf("%s", file_entry_get_name(entry));
        return;
    }

    t_file_stats *target_stats = NULL;
    const char *color = get_entry_color(entry, &target_stats);
    print_colored_text(file_entry_get_name(entry), color);
    file_stats_destroy(&target_stats);
}

void render_colors_print_name_with_target(const t_parsed_arguments *parsed_arguments, const t_file_entry *entry)
{
    if (entry == NULL)
        return;

    const char *name = file_entry_get_name(entry);
    const char *target = file_entry_get_link_target(entry);
    if (!color_is_enabled(parsed_arguments))
    {
        ft_printf("%s", name);
        if (target != NULL)
            ft_printf(" -> %s", target);
        return;
    }

    t_file_stats *target_stats = NULL;
    const char *entry_color = get_entry_color(entry, &target_stats);
    print_colored_text(name, entry_color);
    if (target != NULL)
    {
        ft_printf(" -> ");
        const char *target_color = target_stats == NULL
            ? COLOR_ORPHAN
            : get_stats_color(target, target_stats);
        print_colored_text(target, target_color);
    }
    file_stats_destroy(&target_stats);
}
