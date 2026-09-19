#include <stdlib.h>
#include "parsed_arguments.h"
#include "file_operands.h"
#include "sorter.h"
#include "libft.h"

struct s_parsed_arguments
{
    char **file_operands;
    t_file_type *file_operand_types;
    char **non_directory_file_operands;
    char **directory_file_operands;
};

static int compare_by_name(const void *first_str, const void *second_str)
{
    return ft_strcmp(first_str, second_str);
}

static int has_failed_to_access_a_file_operand(const unsigned int num_of_operands, const t_file_type *file_operands_types)
{
    for (unsigned int i = 0; i < num_of_operands; i++)
    {
        if (file_operands_types[i] == FILE_TYPE_UNKNOWN)
            return 1;
    }

    return 0;
}

t_result *parse_arguments(const int num_of_arguments, const char **arguments)
{
    if (num_of_arguments < 0 || arguments == NULL)
        return NULL;

    t_parsed_arguments *parsed_arguments = ft_safe_calloc(1, sizeof(t_parsed_arguments));
    parsed_arguments->file_operands = file_operands_get(num_of_arguments, arguments);
    parsed_arguments->file_operand_types = file_operands_get_types(parsed_arguments->file_operands);
    parsed_arguments->non_directory_file_operands = file_operands_get_non_directory(parsed_arguments->file_operands, parsed_arguments->file_operand_types);
    parsed_arguments->directory_file_operands = file_operands_get_directory(parsed_arguments->file_operands, parsed_arguments->file_operand_types);
    sort_pointer_array((void **) parsed_arguments->non_directory_file_operands, compare_by_name);
    sort_pointer_array((void **) parsed_arguments->directory_file_operands, compare_by_name);

    if (has_failed_to_access_a_file_operand(num_of_arguments, parsed_arguments->file_operand_types))
        return result_create_failed(parsed_arguments);

    return result_create_successful(parsed_arguments);
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

int parsed_arguments_has_directory_file_operands(const t_parsed_arguments *parsed_arguments)
{
    if (parsed_arguments == NULL)
        return 0;

    return parsed_arguments->directory_file_operands[0] != NULL;
}

int parsed_arguments_has_mixed_types_file_operands(const t_parsed_arguments *parsed_arguments)
{
    if (parsed_arguments == NULL)
        return 0;

    return parsed_arguments->non_directory_file_operands[0] != NULL && parsed_arguments->directory_file_operands[0] != NULL;
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
