#include <stdlib.h>
#include "file_entry.h"
#include "sorter.h"
#include "libft.h"

#define DEFAULT_CAPACITY 8

struct s_file_entry
{
    char *name;
    long blocks;
    t_permissions permissions;
    t_file_type type;
    unsigned long link_count;
    char *author;
    char *group;
    long size;
    t_device device;
    t_date modification_date;
    char *target;
};

struct s_file_entry_array
{
    t_file_entry **entries;
    unsigned int count;
    unsigned int max_capacity;
    long total_blocks;
    unsigned long biggest_link_count;
    size_t biggest_author_width;
    size_t biggest_group_width;
    long biggest_size;
    size_t biggest_device_width;
    size_t biggest_date_width;
};

static size_t count_unsigned_digits(unsigned long number)
{
    size_t digit_count = 1;

    while (number >= 10)
    {
        number /= 10;
        digit_count++;
    }
    return digit_count;
}

static int compare_by_entry_name(const void *first_entry, const void *second_entry)
{
    const char *first_entry_name = file_entry_get_name(first_entry);
    const char *second_entry_name = file_entry_get_name(second_entry);

    return ft_strcmp(first_entry_name, second_entry_name);
}

static void increase_entries_capacity(t_file_entry_array *array)
{
    const unsigned int old_capacity = array->max_capacity;
    const unsigned int new_capacity = array->max_capacity * 2;

    array->max_capacity = new_capacity;
    array->entries = ft_safe_realloc(array->entries,
                                    (old_capacity + 1) * sizeof(t_file_entry *),
                                    (new_capacity + 1) * sizeof(t_file_entry *));
}

t_file_entry *file_entry_create(const char *file_name)
{
    if (!ft_is_valid_path(file_name))
        return NULL;

    t_file_entry *file_entry = ft_safe_calloc(1, sizeof(t_file_entry));
    file_entry_set_name(file_entry, file_name);
    file_entry->blocks = 0;
    file_entry->permissions = PERM_NONE;
    file_entry->type = FILE_TYPE_UNKNOWN;
    file_entry->link_count = 0;

    return file_entry;
}

t_file_entry_array *file_entry_array_create(void)
{
    t_file_entry_array *file_entry_array = ft_safe_calloc(1, sizeof(t_file_entry_array));

    file_entry_array->entries = ft_safe_calloc(DEFAULT_CAPACITY + 1, sizeof(t_file_entry *));
    file_entry_array->count = 0;
    file_entry_array->biggest_date_width = 1;
    file_entry_array->max_capacity = DEFAULT_CAPACITY;

    return file_entry_array;
}

const char *file_entry_get_name(const t_file_entry *file_entry)
{
    if (file_entry == NULL)
        return NULL;

    return file_entry->name;
}

void file_entry_set_name(t_file_entry *entry, const char *name)
{
    if (entry == NULL || !ft_is_valid_path(name))
        return;

    free(entry->name);
    entry->name = ft_safe_strdup(name);
}

long file_entry_get_blocks(const t_file_entry *entry)
{
    if (entry == NULL)
        return 0;

    return entry->blocks;
}

void file_entry_set_blocks(t_file_entry *entry, const long blocks)
{
    if (entry == NULL)
        return ;

    entry->blocks = blocks;
}

t_permissions file_entry_get_permissions(const t_file_entry *entry)
{
    if (entry == NULL)
        return PERM_NONE;

    return entry->permissions;
}

void file_entry_set_permissions(t_file_entry *entry, const t_permissions permissions)
{
    if (entry == NULL)
        return ;

    entry->permissions = permissions;
}

t_file_type file_entry_get_file_type(const t_file_entry *entry)
{
    if (entry == NULL)
        return FILE_TYPE_UNKNOWN;

    return entry->type;
}

void file_entry_set_file_type(t_file_entry *entry, const t_file_type file_type)
{
    if (entry == NULL || file_type == FILE_TYPE_NONE)
        return ;

    entry->type = file_type;
}

unsigned long file_entry_get_link_count(const t_file_entry *entry)
{
    if (entry == NULL)
        return 0;

    return entry->link_count;
}

void file_entry_set_link_count(t_file_entry *entry, const unsigned long link_count)
{
    if (entry == NULL)
        return ;

    entry->link_count = link_count;
}

const char *file_entry_get_author(const t_file_entry *entry)
{
    if (entry == NULL)
        return NULL;

    return entry->author;
}

void file_entry_set_author(t_file_entry *entry, const char *author)
{
    if (entry == NULL || author == NULL)
        return ;

    free(entry->author);
    entry->author = ft_safe_strdup(author);
}

const char *file_entry_get_group(const t_file_entry *entry)
{
    if (entry == NULL)
        return NULL;

    return entry->group;
}

void file_entry_set_group(t_file_entry *entry, const char *group)
{
    if (entry == NULL || group == NULL)
        return ;

    free(entry->group);
    entry->group = ft_safe_strdup(group);
}

long file_entry_get_size(const t_file_entry *entry)
{
    if (entry == NULL)
        return 0;

    return entry->size;
}

void file_entry_set_size(t_file_entry *entry, const long size)
{
    if (entry == NULL)
        return ;

    entry->size = size;
}

t_device file_entry_get_device(const t_file_entry *entry)
{
    if (entry == NULL)
        return (t_device){0};

    return entry->device;
}

void file_entry_set_device(t_file_entry *entry, const t_device device)
{
    if (entry == NULL)
        return ;

    entry->device = device;
}

t_date file_entry_get_modification_date(const t_file_entry *entry)
{
    if (entry == NULL)
        return (t_date){0};

    return entry->modification_date;
}

void file_entry_set_modification_date(t_file_entry *entry, const t_date modification_date)
{
    if (entry == NULL)
        return ;

    entry->modification_date = modification_date;
}

const char *file_entry_get_link_target(const t_file_entry *entry)
{
    if (entry == NULL)
        return NULL;

    return entry->target;
}

void file_entry_set_link_target(t_file_entry *entry, const char *target)
{
    if (entry == NULL || entry->type != FILE_TYPE_SYMLINK || !ft_is_valid_path(target))
        return ;

    free(entry->target);
    entry->target = ft_safe_strdup(target);
}

const t_file_entry *file_entry_array_get_at(const t_file_entry_array *file_entry_array, const unsigned int index)
{
    if (file_entry_array == NULL || index >= file_entry_array->count)
        return NULL;

    return file_entry_array->entries[index];
}

void file_entry_array_push(t_file_entry_array *array, t_file_entry *entry)
{
    if (array == NULL || entry == NULL)
        return;

    if (array->count >= array->max_capacity)
        increase_entries_capacity(array);
    array->entries[array->count] = entry;
    array->count++;

    array->total_blocks += entry->blocks;

    if (entry->link_count > array->biggest_link_count)
        array->biggest_link_count = entry->link_count;

    const size_t author_width = entry->author != NULL ? ft_strlen(entry->author) : 0;
    if (author_width > array->biggest_author_width)
        array->biggest_author_width = author_width;

    const size_t group_width = entry->group != NULL ? ft_strlen(entry->group) : 0;
    if (group_width > array->biggest_group_width)
        array->biggest_group_width = group_width;

    if (entry->size > array->biggest_size)
        array->biggest_size = entry->size;

    if (entry->type == FILE_TYPE_CHARDEVICE || entry->type == FILE_TYPE_BLOCKDEVICE)
    {
        const size_t device_width = count_unsigned_digits(entry->device.major) + count_unsigned_digits(entry->device.minor);
        if (device_width > array->biggest_device_width)
            array->biggest_device_width = device_width;
    }

    if (entry->type != FILE_TYPE_UNKNOWN)
        array->biggest_date_width = 12;
}

unsigned int file_entry_array_get_length(const t_file_entry_array *file_entry_array)
{
    if (file_entry_array == NULL)
        return 0;

    return file_entry_array->count;
}

long file_entry_array_get_total_blocks(const t_file_entry_array *file_entry_array)
{
    if (file_entry_array == NULL)
        return 0;

    return file_entry_array->total_blocks;
}

size_t file_entry_array_get_biggest_link_count(const t_file_entry_array *file_entry_array)
{
    if (file_entry_array == NULL || file_entry_array->count == 0)
        return 0;

    return count_unsigned_digits(file_entry_array->biggest_link_count);
}

size_t file_entry_array_get_biggest_author_width(const t_file_entry_array *file_entry_array)
{
    if (file_entry_array == NULL)
        return 0;

    return file_entry_array->biggest_author_width;
}

size_t file_entry_array_get_biggest_group_width(const t_file_entry_array *file_entry_array)
{
    if (file_entry_array == NULL)
        return 0;

    return file_entry_array->biggest_group_width;
}

size_t file_entry_array_get_biggest_size(const t_file_entry_array *file_entry_array)
{
    if (file_entry_array == NULL || file_entry_array->count == 0)
        return 0;

    return count_unsigned_digits((unsigned long)file_entry_array->biggest_size);
}

size_t file_entry_array_get_biggest_device_width(const t_file_entry_array *file_entry_array)
{
    if (file_entry_array == NULL)
        return 0;

    return file_entry_array->biggest_device_width;
}

size_t file_entry_array_get_biggest_date_width(const t_file_entry_array *file_entry_array)
{
    if (file_entry_array == NULL)
        return 0;

    return file_entry_array->biggest_date_width;
}

void file_entry_array_sort(const t_file_entry_array *file_entry_array)
{
    if (file_entry_array == NULL)
        return ;

    sort_pointer_array((void **) file_entry_array->entries, compare_by_entry_name);
}

void file_entry_destroy(t_file_entry **file_entry)
{
    if (file_entry == NULL || *file_entry == NULL)
        return;

    free((*file_entry)->name);
    free((*file_entry)->author);
    free((*file_entry)->group);
    free((*file_entry)->target);
    free((*file_entry));
    *file_entry = NULL;
}

void file_entry_array_destroy(t_file_entry_array **file_entry_array)
{
    if (file_entry_array == NULL || *file_entry_array == NULL)
        return;

    for (unsigned int i = 0; i < (*file_entry_array)->count; i++)
    {
        t_file_entry *entry = (*file_entry_array)->entries[i];
        file_entry_destroy(&entry);
    }
    free((*file_entry_array)->entries);
    free(*file_entry_array);
    *file_entry_array = NULL;
}
