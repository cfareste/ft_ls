#include <stdlib.h>
#include "parsed_arguments.h"
#include "libft.h"

#define DEFAULT_NUM_OF_OPERANDS 1
#define DEFAULT_FILE_OPERAND "."

struct s_parsed_arguments
{
    char **file_operands;
};

static char **get_default_file_operands()
{
    char **file_operands = ft_safe_calloc(DEFAULT_NUM_OF_OPERANDS + 1, sizeof(char *));

    file_operands[0] = ft_safe_strdup(DEFAULT_FILE_OPERAND);

    return file_operands;
}

static char **get_file_operands_from_arguments(const int num_of_arguments, const char **arguments)
{
    char **file_operands = ft_safe_calloc(num_of_arguments + 1, sizeof(char *));

    for (int i = 0; i < num_of_arguments; i++)
    {
        file_operands[i] = ft_safe_strdup(arguments[i]);
    }

    return file_operands;
}

static char **get_file_operands(const int num_of_arguments, const char **arguments)
{
    if (num_of_arguments == 0)
        return get_default_file_operands();

    return get_file_operands_from_arguments(num_of_arguments, arguments);
}

t_parsed_arguments *parse_arguments(const int num_of_arguments, const char **arguments)
{
    if (num_of_arguments < 0 || arguments == NULL)
        return NULL;

    t_parsed_arguments *parsed_arguments = ft_safe_calloc(1, sizeof(t_parsed_arguments));
    parsed_arguments->file_operands = get_file_operands(num_of_arguments, arguments);

    return parsed_arguments;
}

const char **parsed_arguments_get_file_operands(const t_parsed_arguments *parsed_arguments)
{
    if (parsed_arguments == NULL)
        return NULL;

    return (const char **) parsed_arguments->file_operands;
}

void parsed_arguments_destroy(t_parsed_arguments **parsed_arguments)
{
    if (parsed_arguments == NULL || *parsed_arguments == NULL)
        return ;

    ft_free_str_matrix((*parsed_arguments)->file_operands);
    free(*parsed_arguments);

    *parsed_arguments = NULL;
}
