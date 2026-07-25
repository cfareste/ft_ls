#include "file_data.h"
#include "libft.h"


struct s_file_data {
    t_file_data *next;
};

t_file_data *file_data_create()
{
    return ft_calloc(1, sizeof(t_file_data));
}

void file_data_destroy(t_file_data **_file_data)
{
    if (!_file_data || !*_file_data)
        return;

    free(*_file_data);
    *_file_data = NULL;
}

void file_data_add_entry(t_file_data *_file_data, t_file_data *_next)
{
    if (_file_data == NULL)
        return;

    while (_file_data->next != NULL)
    {
        _file_data = _file_data->next;
    }

    _file_data->next = _next;
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
