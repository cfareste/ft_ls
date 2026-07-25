#include "file_data.h"

#include "libft.h"


struct s_file_data {};

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
