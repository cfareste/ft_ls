#include <stdlib.h>
#include "parsed_arguments.h"
#include "file_operands.h"
#include "sorter.h"
#include "libft.h"

struct s_parsed_arguments
{
    t_options options;
    char **file_operands;
    t_file_stats **file_operands_stats;
    t_file_stats **non_directory_file_operands_stats;
    int has_implicit_default_operand;
    char **non_directory_file_operands;
    char **directory_file_operands;
};

static int compare_by_name(const void *first_str, const void *second_str)
{
    return ft_strcmp(first_str, second_str);
}

static t_file_stats **get_non_directory_file_operands_stats(char **non_directory_file_operands, char **file_operands, t_file_stats **file_operands_stats)
{
    unsigned int count = 0;

    while (non_directory_file_operands[count] != NULL)
        count++;

    t_file_stats **non_directory_file_operands_stats = ft_safe_calloc(count + 1, sizeof(t_file_stats *));

    for (unsigned int i = 0; i < count; i++)
    {
        for (unsigned int j = 0; file_operands[j] != NULL; j++)
        {
            if (ft_are_string_equals(non_directory_file_operands[i], file_operands[j]))
            {
                non_directory_file_operands_stats[i] = file_operands_stats[j];
                break;
            }
        }
    }
    return non_directory_file_operands_stats;
}

static int has_failed_to_access_a_file_operand(char **file_operands, t_file_stats **file_operands_stats, const int has_implicit_default_operand)
{
    if (has_implicit_default_operand)
        return 0;

    for (unsigned int i = 0; file_operands[i] != NULL; i++)
    {
        if (file_stats_get_file_type(file_operands_stats[i]) == FILE_TYPE_UNKNOWN)
        {
            return 1;
        }
    }

    return 0;
}

static t_result *create_parsing_arguments_result(t_parsed_arguments *parsed_arguments)
{
    if (has_failed_to_access_a_file_operand(parsed_arguments->file_operands, parsed_arguments->file_operands_stats, parsed_arguments->has_implicit_default_operand))
        return result_create_failed(parsed_arguments, NULL);

    return result_create_successful(parsed_arguments);
}

static void retrieve_advanced_file_stats(t_file_stats **file_operands_stats)
{
    for (unsigned int i = 0; file_operands_stats[i] != NULL; i++)
    {
        file_stats_get_author(file_operands_stats[i]);
        file_stats_get_group(file_operands_stats[i]);
        file_stats_get_target_pointed_by_link(file_operands_stats[i]);
    }
}

t_result *parse_arguments(const int num_of_arguments, const char **arguments)
{
    if (num_of_arguments < 0 || arguments == NULL)
        return NULL;

    t_parsed_arguments *parsed_arguments = ft_safe_calloc(1, sizeof(t_parsed_arguments));
    const int num_of_file_operands = num_of_arguments - options_get_amount(arguments);

    parsed_arguments->options = options_get(arguments);
    parsed_arguments->has_implicit_default_operand = num_of_file_operands == 0;
    parsed_arguments->file_operands = file_operands_get(num_of_file_operands, arguments);
    parsed_arguments->file_operands_stats = file_operands_get_stats(num_of_file_operands, parsed_arguments->file_operands);
    if (parsed_arguments_is_long_format_active(parsed_arguments))
        retrieve_advanced_file_stats(parsed_arguments->file_operands_stats);
    parsed_arguments->non_directory_file_operands = file_operands_get_non_directory(parsed_arguments->file_operands, parsed_arguments->file_operands_stats, parsed_arguments->has_implicit_default_operand);
    parsed_arguments->directory_file_operands = file_operands_get_directory(parsed_arguments->file_operands, parsed_arguments->file_operands_stats, parsed_arguments->has_implicit_default_operand);
    sort_pointer_array((void **) parsed_arguments->non_directory_file_operands, compare_by_name);
    sort_pointer_array((void **) parsed_arguments->directory_file_operands, compare_by_name);
    parsed_arguments->non_directory_file_operands_stats = get_non_directory_file_operands_stats(parsed_arguments->non_directory_file_operands, parsed_arguments->file_operands, parsed_arguments->file_operands_stats);

    return create_parsing_arguments_result(parsed_arguments);
}

const char * const *parsed_arguments_get_non_directory_file_operands(const t_parsed_arguments *parsed_arguments)
{
    if (parsed_arguments == NULL)
        return NULL;

    return (const char * const *) parsed_arguments->non_directory_file_operands;
}

t_file_stats * const *parsed_arguments_get_non_directory_file_operands_stats(const t_parsed_arguments *parsed_arguments)
{
    if (parsed_arguments == NULL)
        return NULL;

    return parsed_arguments->non_directory_file_operands_stats;
}

const char * const *parsed_arguments_get_directory_file_operands(const t_parsed_arguments *parsed_arguments)
{
    if (parsed_arguments == NULL)
        return NULL;

    return (const char * const *) parsed_arguments->directory_file_operands;
}

int parsed_arguments_has_any_option_active(const t_parsed_arguments *parsed_arguments)
{
    if (parsed_arguments == NULL)
        return 0;

    return parsed_arguments->options != OPTIONS_NONE;
}

int parsed_arguments_is_option_active(const t_parsed_arguments *parsed_arguments, const t_options option)
{
    if (parsed_arguments == NULL)
        return 0;

    return (parsed_arguments->options & option) != OPTIONS_NONE;
}

int parsed_arguments_is_long_format_active(const t_parsed_arguments *parsed_arguments)
{
    return parsed_arguments_is_option_active(parsed_arguments, OPTIONS_LONG_FORMAT);
}

int parsed_arguments_is_file_operand(const t_parsed_arguments *parsed_arguments, const char *file)
{
    if (parsed_arguments == NULL || !ft_is_valid_path(file))
        return 0;

    for (unsigned int i = 0; parsed_arguments->file_operands[i] != NULL; i++)
    {
        if (ft_are_string_equals(parsed_arguments->file_operands[i], file))
        {
            return 1;
        }
    }

    return 0;
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

    for (unsigned int i = 0; (*parsed_arguments)->file_operands[i] != NULL; i++)
        file_stats_destroy(&(*parsed_arguments)->file_operands_stats[i]);
    free((*parsed_arguments)->file_operands_stats);
    ft_free_str_matrix((*parsed_arguments)->file_operands);
    ft_free_str_matrix((*parsed_arguments)->non_directory_file_operands);
    free((*parsed_arguments)->non_directory_file_operands_stats);
    ft_free_str_matrix((*parsed_arguments)->directory_file_operands);
    free(*parsed_arguments);

    *parsed_arguments = NULL;
}
