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
    char *name;
    t_file_entry_array *next;
    //
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

const char *file_entry_array_get_name(const t_file_entry_array *file_entry_array)
{
    if (file_entry_array == NULL)
        return NULL;

    return file_entry_array->name;
}

void file_entry_set_name(t_file_entry *entry, const char *name)
{
    if (entry == NULL || !is_valid_file_name(name))
        return;

    free(entry->name);
    entry->name = ft_safe_strdup(name);
}

void file_entry_array_set_name(t_file_entry_array *file_entry_array, const char *name)
{
    if (file_entry_array == NULL || !is_valid_file_name(name))
        return;

    free(file_entry_array->name);
    file_entry_array->name = ft_safe_strdup(name);
}

const t_file_entry *file_entry_array_get_at(const t_file_entry_array *file_entry_array, const unsigned int index)
{
    if (file_entry_array == NULL)
        return NULL;

    return file_entry_array->entries[index];
}

t_file_entry_array *file_entry_array_get_next(const t_file_entry_array *file_entry_array)
{
    if (file_entry_array == NULL)
        return NULL;

    return file_entry_array->next;
}

void file_entry_array_push_TEMP(t_file_entry_array *array, t_file_entry *entry)
{
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

void file_entry_array_push(t_file_entry_array **file_entry_array, t_file_entry_array *next)
{
    if (file_entry_array == NULL)
        return;

    if (*file_entry_array == NULL)
    {
        *file_entry_array = next;
        return;
    }

    t_file_entry_array *temp = *file_entry_array;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = next;
}

unsigned int file_entry_array_get_length_TEMP(const t_file_entry_array *file_entry_array)
{
    return file_entry_array->count;
}

unsigned int file_entry_array_get_length(const t_file_entry_array *file_entry_array)
{
    unsigned int count = 0;

    while (file_entry_array != NULL)
    {
        count++;
        file_entry_array = file_entry_array->next;
    }

    return count;
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

    const t_file_entry_array *file_entry_array_temp = *file_entry_array;

    for (unsigned int i = 0; i < file_entry_array_temp->count; i++)
    {
        file_entry_destroy(&file_entry_array_temp->entries[i]);
    }

    free(file_entry_array_temp->entries);


    t_file_entry_array *to_free = *file_entry_array;
    t_file_entry_array *next = to_free->next;
    do
    {
        free(to_free->name);
        free(to_free);
        to_free = next;
        if (next != NULL)
            next = next->next;
    } while (to_free != NULL);

    *file_entry_array = NULL;
}
