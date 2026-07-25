#include "file_data.h"

#include <stddef.h>

#include "libft.h"

struct s_file_data {};

t_file_data *file_data_create()
{
    return ft_calloc(1, sizeof(t_file_data));
}