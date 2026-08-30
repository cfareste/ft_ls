#include <stdlib.h>
#include "file_entry.h"
#include "libft.h"

#define DEFAULT_CAPACITY 8

struct s_file_entry
{
    char *name;
};

struct s_file_entry_array
{
    t_file_entry **entries;
    unsigned int count;
    unsigned int max_capacity;
};

static int is_valid_file_name(const char *name)
{
    return (name != NULL && !ft_is_str_empty(name));
}

t_file_entry *file_entry_create(const char *file_name)
{
    if (!is_valid_file_name(file_name))
        return NULL;

    t_file_entry *file_entry = ft_safe_calloc(1, sizeof(t_file_entry));
    file_entry_set_name(file_entry, file_name);

    return file_entry;
}

t_file_entry_array *file_entry_array_create()
{
    t_file_entry_array *file_entry_array = ft_safe_calloc(1, sizeof(t_file_entry_array));

    file_entry_array->entries = ft_safe_calloc(DEFAULT_CAPACITY, sizeof(t_file_entry *));
    file_entry_array->count = 0;
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
    if (entry == NULL || !is_valid_file_name(name))
        return;

    free(entry->name);
    entry->name = ft_safe_strdup(name);
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
    {
        array->max_capacity *= 2;
        array->entries = ft_realloc(array->entries,
                            (array->max_capacity / 2) * sizeof(t_file_entry *),
                            array->max_capacity * sizeof(t_file_entry *));
    }
    array->entries[array->count] = entry;
    array->count++;
}

unsigned int file_entry_array_get_length(const t_file_entry_array *file_entry_array)
{
    if (file_entry_array == NULL)
        return 0;

    return file_entry_array->count;
}

void file_entry_destroy(t_file_entry **file_entry)
{
    if (file_entry == NULL || *file_entry == NULL)
        return;

    free((*file_entry)->name);
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
