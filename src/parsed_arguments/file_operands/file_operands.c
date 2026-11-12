#include <stddef.h>
#include "file_operands.h"
#include "libft.h"

#define DEFAULT_NUM_OF_OPERANDS 1
#define DEFAULT_FILE_OPERAND "."

typedef enum e_file_operand_category
{
    FILE_OPERAND_DIRECTORY,
    FILE_OPERAND_NON_DIRECTORY
} t_file_operand_category;

static int argument_is_an_option(const char *argument)
{
    return argument[0] == '-';
}

static char **get_default_file_operands(void)
{
    char **file_operands = ft_safe_calloc(DEFAULT_NUM_OF_OPERANDS + 1, sizeof(char *));

    file_operands[0] = ft_safe_strdup(DEFAULT_FILE_OPERAND);

    return file_operands;
}

static char **get_file_operands_from_arguments(const int num_of_file_operands, const char **arguments)
{
    char **file_operands = ft_safe_calloc(num_of_file_operands + 1, sizeof(char *));

    int operands_i = 0;
    int arguments_i = 0;
    while (arguments[arguments_i] != NULL)
    {
        if (!argument_is_an_option(arguments[arguments_i]))
        {
            file_operands[operands_i] = ft_safe_strdup(arguments[arguments_i]);
            operands_i++;
        }
        arguments_i++;
    }

    return file_operands;
}

static t_file_stats **get_stats_from_file_operands(const int num_of_file_operands, char **file_operands, const int follow_symlinks)
{
    t_file_stats **file_operands_stats = ft_safe_calloc(num_of_file_operands + 1, sizeof(t_file_stats *));
    t_file_stats *(*get_stats)(const char *) = follow_symlinks ? file_stats_get : file_stats_get_without_following_symlinks;

    for (int i = 0; i < num_of_file_operands; i++)
        file_operands_stats[i] = get_stats(file_operands[i]);

    return file_operands_stats;
}

static t_file_type get_file_operand_type(t_file_stats **file_operands_stats, const unsigned int index, const int has_implicit_default_operand)
{
    if (has_implicit_default_operand && index == 0)
        return FILE_TYPE_DIRECTORY;
    return file_stats_get_file_type(file_operands_stats[index]);
}

static int file_operand_matches_category(const t_file_type file_type, const t_file_operand_category category)
{
    if (category == FILE_OPERAND_DIRECTORY)
        return file_type == FILE_TYPE_DIRECTORY;
    return file_type != FILE_TYPE_DIRECTORY && file_type != FILE_TYPE_UNKNOWN;
}

static char **get_file_operands_by_category(char **file_operands, t_file_stats **file_operands_stats, const int has_implicit_default_operand, const t_file_operand_category category)
{
    unsigned int matching_operands_count = 0;

    for (unsigned int i = 0; file_operands[i] != NULL; i++)
    {
        const t_file_type file_type = get_file_operand_type(file_operands_stats, i, has_implicit_default_operand);
        if (file_operand_matches_category(file_type, category))
            matching_operands_count++;
    }

    char **matching_file_operands = ft_safe_calloc(matching_operands_count + 1, sizeof(char *));
    unsigned int matching_operand_index = 0;

    for (unsigned int i = 0; file_operands[i] != NULL; i++)
    {
        const t_file_type file_type = get_file_operand_type(file_operands_stats, i, has_implicit_default_operand);
        if (!file_operand_matches_category(file_type, category))
            continue ;

        matching_file_operands[matching_operand_index] = ft_safe_strdup(file_operands[i]);
        matching_operand_index++;
    }

    return matching_file_operands;
}

char **file_operands_get(const int num_of_file_operands, const char **arguments)
{
    if (num_of_file_operands == 0)
        return get_default_file_operands();

    return get_file_operands_from_arguments(num_of_file_operands, arguments);
}

t_file_stats **file_operands_get_stats(const int num_of_file_operands, char **file_operands, const int follow_symlinks)
{
    return get_stats_from_file_operands(num_of_file_operands, file_operands, follow_symlinks);
}

char **file_operands_get_non_directory(char **file_operands, t_file_stats **file_operands_stats, const int has_implicit_default_operand)
{
    return get_file_operands_by_category(file_operands, file_operands_stats, has_implicit_default_operand, FILE_OPERAND_NON_DIRECTORY);
}

char **file_operands_get_directory(char **file_operands, t_file_stats **file_operands_stats, const int has_implicit_default_operand)
{
    return get_file_operands_by_category(file_operands, file_operands_stats, has_implicit_default_operand, FILE_OPERAND_DIRECTORY);
}
