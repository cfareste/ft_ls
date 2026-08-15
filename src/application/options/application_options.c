#include <stdlib.h>
#include "application_options.h"
#include "libft.h"

struct s_ft_ls_options
{
    char **file_parameters;
};

static size_t get_num_of_file_parameters(const int num_of_arguments)
{
    if (num_of_arguments < 1)
        return 1;

    return num_of_arguments;
}

static char **get_file_parameters(const int num_of_arguments, const char **arguments)
{
    const size_t num_of_file_parameters = get_num_of_file_parameters(num_of_arguments);
    char **file_parameters = ft_safe_calloc(num_of_file_parameters + 1, sizeof(const char *));

    if (num_of_arguments == 0)
    {
        file_parameters[0] = ft_safe_strdup(".");
        return file_parameters;
    }

    size_t i = 0;
    for (i = 0; i < num_of_file_parameters; i++)
    {
        file_parameters[i] = ft_safe_strdup(arguments[i]);
    }

    return file_parameters;
}

t_ft_ls_options *ft_ls_options_get(const int num_of_arguments, const char **arguments)
{
    if (num_of_arguments < 0 || arguments == NULL)
        return NULL;

    t_ft_ls_options *options = ft_safe_calloc(1, sizeof(t_ft_ls_options));
    options->file_parameters = get_file_parameters(num_of_arguments, arguments);

    return options;
}

void ft_ls_options_destroy(t_ft_ls_options **options)
{
    if (options == NULL || *options == NULL)
        return ;

    size_t i = 0;
    do
    {
        free((*options)->file_parameters[i]);
        i++;
    } while ((*options)->file_parameters[i] != NULL);
    free((*options)->file_parameters);
    free(*options);
    *options = NULL;
}

const char **ft_ls_options_get_file_parameters(const t_ft_ls_options *options)
{
    if (options == NULL)
        return NULL;

    return (const char **) options->file_parameters;
}
