#include <stdlib.h>
#include "file_data.h"
#include "libft.h"


struct s_file_data {
    char *name;
    t_file_data *next;
};

static int is_valid_file_name(const char *_name)
{
    return (_name && !ft_isstr_empty(_name));
}

t_file_data *file_data_create(const char *file_name)
{
    if (!is_valid_file_name(file_name))
        return NULL;

    t_file_data *file_data = ft_safe_calloc(1, sizeof(t_file_data));

    file_data_set_name(file_data, file_name);

    return file_data;
}

const char *file_data_get_name(const t_file_data *_file_data)
{
    if (_file_data == NULL)
        return NULL;

    return _file_data->name;
}

void file_data_set_name(t_file_data *_file_data, const char *_name)
{
    if (_file_data == NULL || !is_valid_file_name(_name))
        return;

    free(_file_data->name);
    _file_data->name = ft_strdup(_name);
}

t_file_data *file_data_get_next(const t_file_data *_file_data)
{
    if (_file_data == NULL)
        return NULL;

    return _file_data->next;
}

void file_data_add_entry(t_file_data **_file_data, t_file_data *_next)
{
    if (_file_data == NULL)
        return;

    if (*_file_data == NULL)
    {
        *_file_data = _next;
        return;
    }

    t_file_data *temp = *_file_data;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = _next;
}

unsigned int file_data_get_length(const t_file_data *_file_data)
{
    unsigned int count = 0;

    while (_file_data != NULL)
    {
        count++;
        _file_data = _file_data->next;
    }

    return count;
}

void file_data_destroy(t_file_data **_file_data)
{
    if (!_file_data || !*_file_data)
        return;

    t_file_data *to_free = *_file_data;
    t_file_data *next = to_free->next;
    do
    {
        free(to_free->name);
        free(to_free);
        to_free = next;
        if (next != NULL)
            next = next->next;
    } while (to_free != NULL);

    *_file_data = NULL;
}
