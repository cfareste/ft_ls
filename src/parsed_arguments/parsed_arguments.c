#include <stdlib.h>
#include "parsed_arguments.h"
#include "file_operands.h"
#include "libft.h"

struct s_parsed_arguments
{
    char **file_operands;
    unsigned int *file_operand_types;
    char **non_directory_file_operands;
    char **directory_file_operands;
};

t_parsed_arguments *parse_arguments(const int num_of_arguments, const char **arguments)
{
    if (num_of_arguments < 0 || arguments == NULL)
        return NULL;

    t_parsed_arguments *parsed_arguments = ft_safe_calloc(1, sizeof(t_parsed_arguments));
    parsed_arguments->file_operands = get_file_operands(num_of_arguments, arguments);
    parsed_arguments->file_operand_types = get_file_operand_types(parsed_arguments->file_operands);
    parsed_arguments->non_directory_file_operands = file_operands_get_non_directory(parsed_arguments->file_operands, parsed_arguments->file_operand_types);
    parsed_arguments->directory_file_operands = file_operands_get_directory(parsed_arguments->file_operands, parsed_arguments->file_operand_types);

    return parsed_arguments;
}

const char * const *parsed_arguments_get_file_operands(const t_parsed_arguments *parsed_arguments)
{
    if (parsed_arguments == NULL)
        return NULL;

    return (const char * const *) parsed_arguments->file_operands;
}

const char * const *parsed_arguments_get_non_directory_file_operands(const t_parsed_arguments *parsed_arguments)
{
    if (parsed_arguments == NULL)
        return NULL;

    return (const char * const *) parsed_arguments->non_directory_file_operands;
}

const char * const *parsed_arguments_get_directory_file_operands(const t_parsed_arguments *parsed_arguments)
{
    if (parsed_arguments == NULL)
        return NULL;

    return (const char * const *) parsed_arguments->directory_file_operands;
}

int parsed_arguments_has_multiple_file_operands(const t_parsed_arguments *parsed_arguments)
{
    if (parsed_arguments == NULL)
        return 0;

    return parsed_arguments->file_operands[1] != NULL;
}

void parsed_arguments_destroy(t_parsed_arguments **parsed_arguments)
{
    if (parsed_arguments == NULL || *parsed_arguments == NULL)
        return ;

    free((*parsed_arguments)->file_operand_types);
    ft_free_str_matrix((*parsed_arguments)->file_operands);
    ft_free_str_matrix((*parsed_arguments)->non_directory_file_operands);
    ft_free_str_matrix((*parsed_arguments)->directory_file_operands);
    free(*parsed_arguments);

    *parsed_arguments = NULL;
}
