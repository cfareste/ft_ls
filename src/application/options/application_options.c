#include <stdlib.h>
#include "application_options.h"
#include "libft.h"

struct s_ft_ls_options
{
    const char **file_parameters;
};

t_ft_ls_options *ft_ls_options_get(const int num_of_arguments, const char **arguments)
{
    if (num_of_arguments < 0 || arguments == NULL)
        return NULL;

    t_ft_ls_options *options = ft_safe_calloc(1, sizeof(t_ft_ls_options));
    options->file_parameters = arguments;

    return options;
}

void ft_ls_options_destroy(t_ft_ls_options **options)
{
    if (options == NULL)
        return ;

    free(*options);
    *options = NULL;
}

const char **ft_ls_options_get_file_parameters(const t_ft_ls_options *options)
{
    return options->file_parameters;
}
