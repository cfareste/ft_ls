#include <stdlib.h>
#include "file_data.h"
#include "libft.h"

struct s_file_entry_list
{
    char *name;
    t_file_entry_list *next;
};

static int is_valid_file_name(const char *_name)
{
    return (_name != NULL && !ft_is_str_empty(_name));
}

t_file_entry_list *file_entry_list_create(const char *file_name)
{
    if (!is_valid_file_name(file_name))
        return NULL;

    t_file_entry_list *file_entry_list = ft_safe_calloc(1, sizeof(t_file_entry_list));
    file_entry_list_set_name(file_entry_list, file_name);

    return file_entry_list;
}

const char *file_entry_list_get_name(const t_file_entry_list *file_entry_list)
{
    if (file_entry_list == NULL)
        return NULL;

    return file_entry_list->name;
}

void file_entry_list_set_name(t_file_entry_list *file_entry_list, const char *name)
{
    if (file_entry_list == NULL || !is_valid_file_name(name))
        return;

    free(file_entry_list->name);
    file_entry_list->name = ft_safe_strdup(name);
}

t_file_entry_list *file_entry_list_get_next(const t_file_entry_list *file_entry_list)
{
    if (file_entry_list == NULL)
        return NULL;

    return file_entry_list->next;
}

void file_entry_list_add_entry(t_file_entry_list **file_entry_list, t_file_entry_list *next)
{
    if (file_entry_list == NULL)
        return;

    if (*file_entry_list == NULL)
    {
        *file_entry_list = next;
        return;
    }

    t_file_entry_list *temp = *file_entry_list;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = next;
}

unsigned int file_entry_list_get_length(const t_file_entry_list *file_entry_list)
{
    unsigned int count = 0;

    while (file_entry_list != NULL)
    {
        count++;
        file_entry_list = file_entry_list->next;
    }

    return count;
}

void file_entry_list_destroy(t_file_entry_list **file_entry_list)
{
    if (file_entry_list == NULL || *file_entry_list == NULL)
        return;

    t_file_entry_list *to_free = *file_entry_list;
    t_file_entry_list *next = to_free->next;
    do
    {
        free(to_free->name);
        free(to_free);
        to_free = next;
        if (next != NULL)
            next = next->next;
    } while (to_free != NULL);

    *file_entry_list = NULL;
}
