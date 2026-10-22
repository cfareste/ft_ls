#include <stddef.h>
#include "file_operands.h"
#include "libft.h"

#define DEFAULT_NUM_OF_OPERANDS 1
#define DEFAULT_FILE_OPERAND "."

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

static t_file_stats **get_default_file_operand_stats(void)
{
    return ft_safe_calloc(DEFAULT_NUM_OF_OPERANDS + 1, sizeof(t_file_stats *));
}

static t_file_stats **get_stats_from_file_operands(const int num_of_file_operands, char **file_operands)
{
    t_file_stats **file_operands_stats = ft_safe_calloc(num_of_file_operands + 1, sizeof(t_file_stats *));

    for (int i = 0; i < num_of_file_operands; i++)
        file_operands_stats[i] = file_stats_get(file_operands[i]);

    return file_operands_stats;
}

static t_file_type get_file_operand_type(t_file_stats **file_operands_stats, const unsigned int index, const int has_implicit_default_operand)
{
    if (has_implicit_default_operand && index == 0)
        return FILE_TYPE_DIRECTORY;
    return file_stats_get_file_type(file_operands_stats[index]);
}

static unsigned int get_num_of_non_directory_file_operands(char **file_operands, t_file_stats **file_operands_stats, const int has_implicit_default_operand)
{
    unsigned int num_of_non_directory_file_operands = 0;

    for (unsigned int i = 0; file_operands[i] != NULL; i++)
    {
        const t_file_type file_type = get_file_operand_type(file_operands_stats, i, has_implicit_default_operand);

        if (file_type == FILE_TYPE_DIRECTORY || file_type == FILE_TYPE_UNKNOWN)
            continue ;

        num_of_non_directory_file_operands++;
    }

    return num_of_non_directory_file_operands;
}

static unsigned int get_num_of_directory_file_operands(char **file_operands, t_file_stats **file_operands_stats, const int has_implicit_default_operand)
{
    unsigned int num_of_directory_file_operands = 0;

    for (unsigned int i = 0; file_operands[i] != NULL; i++)
    {
        if (get_file_operand_type(file_operands_stats, i, has_implicit_default_operand) != FILE_TYPE_DIRECTORY)
            continue ;

        num_of_directory_file_operands++;
    }

    return num_of_directory_file_operands;
}

char **file_operands_get(const int num_of_file_operands, const char **arguments)
{
    if (num_of_file_operands == 0)
        return get_default_file_operands();

    return get_file_operands_from_arguments(num_of_file_operands, arguments);
}

t_file_stats **file_operands_get_stats(const int num_of_file_operands, char **file_operands)
{
    if (num_of_file_operands == 0)
        return get_default_file_operand_stats();

    return get_stats_from_file_operands(num_of_file_operands, file_operands);
}

char **file_operands_get_non_directory(char **file_operands, t_file_stats **file_operands_stats, const int has_implicit_default_operand)
{
    const unsigned int num_of_non_directory_file_operands = get_num_of_non_directory_file_operands(file_operands, file_operands_stats, has_implicit_default_operand);
    char **non_directory_file_operands = ft_safe_calloc(num_of_non_directory_file_operands + 1, sizeof(char *));

    int num_of_non_directory_operands = 0;
    for (int i = 0; file_operands[i] != NULL; i++)
    {
        const t_file_type file_type = get_file_operand_type(file_operands_stats, i, has_implicit_default_operand);

        if (file_type == FILE_TYPE_DIRECTORY || file_type == FILE_TYPE_UNKNOWN)
            continue ;

        non_directory_file_operands[num_of_non_directory_operands] = ft_safe_strdup(file_operands[i]);
        num_of_non_directory_operands++;
    }

    return non_directory_file_operands;
}

char **file_operands_get_directory(char **file_operands, t_file_stats **file_operands_stats, const int has_implicit_default_operand)
{
    const unsigned int num_of_directory_file_operands = get_num_of_directory_file_operands(file_operands, file_operands_stats, has_implicit_default_operand);
    char **directory_file_operands = ft_safe_calloc(num_of_directory_file_operands + 1, sizeof(char *));

    int num_of_directory_operands = 0;
    for (int i = 0; file_operands[i] != NULL; i++)
    {
        if (get_file_operand_type(file_operands_stats, (unsigned int)i, has_implicit_default_operand) != FILE_TYPE_DIRECTORY)
            continue ;

        directory_file_operands[num_of_directory_operands] = ft_safe_strdup(file_operands[i]);
        num_of_directory_operands++;
    }

    return directory_file_operands;
}
