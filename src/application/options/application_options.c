#include <stdlib.h>
#include "application_options.h"
#include "libft.h"

struct s_ft_ls_options
{
    char **file_parameters;
};

static char **get_default_file_operand()
{
    char **file_operands = ft_safe_calloc(2, sizeof(char *));

    file_operands[0] = ft_safe_strdup(".");

    return file_operands;
}

static char **get_file_operands_from_arguments(const int num_of_arguments, const char **arguments)
{
    char **file_parameters = ft_safe_calloc(num_of_arguments + 1, sizeof(char *));

    for (int i = 0; i < num_of_arguments; i++)
    {
        file_parameters[i] = ft_safe_strdup(arguments[i]);
    }

    return file_parameters;
}

static char **get_file_parameters(const int num_of_arguments, const char **arguments)
{
    if (num_of_arguments == 0)
        return get_default_file_operand();

    return get_file_operands_from_arguments(num_of_arguments, arguments);
}

t_ft_ls_options *ft_ls_options_get(const int num_of_arguments, const char **arguments)
{
    if (num_of_arguments < 0 || arguments == NULL)
        return NULL;

    t_ft_ls_options *options = ft_safe_calloc(1, sizeof(t_ft_ls_options));
    options->file_parameters = get_file_parameters(num_of_arguments, arguments);

    return options;
}

const char **ft_ls_options_get_file_parameters(const t_ft_ls_options *options)
{
    if (options == NULL)
        return NULL;

    return (const char **) options->file_parameters;
}

void ft_ls_options_destroy(t_ft_ls_options **options)
{
    if (options == NULL || *options == NULL)
        return ;

    ft_free_str_matrix((*options)->file_parameters);
    free(*options);

    *options = NULL;
}
