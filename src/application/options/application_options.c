#include <stdlib.h>
#include "application_options.h"
#include "libft.h"

struct s_ft_ls_options
{
};

t_ft_ls_options *ft_ls_options_get(const char **arguments)
{
    if (arguments == NULL)
        return NULL;

    return ft_safe_calloc(1, sizeof(t_ft_ls_options));
}

void ft_ls_options_destroy(t_ft_ls_options **options)
{
    free(*options);
    *options = NULL;
}
