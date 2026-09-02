#include <stddef.h>
#include "file_operands.h"
#include "file_stats.h"
#include "libft.h"

#define DEFAULT_NUM_OF_OPERANDS 1
#define DEFAULT_FILE_OPERAND "."

static char **get_default_file_operands(void)
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

static unsigned int get_num_of_non_directory_file_operands(const unsigned int *file_operand_types)
{
    unsigned int num_of_non_directory_file_operands = 0;

    for (unsigned int i = 0; file_operand_types[i] != 0; i++)
    {
        if (!S_ISDIR(file_operand_types[i]))
        {
            num_of_non_directory_file_operands++;
        }
    }

    return num_of_non_directory_file_operands;
}

static unsigned int get_num_of_directory_file_operands(const unsigned int *file_operand_types)
{
    unsigned int num_of_directory_file_operands = 0;

    for (unsigned int i = 0; file_operand_types[i] != 0; i++)
    {
        if (S_ISDIR(file_operand_types[i]))
        {
            num_of_directory_file_operands++;
        }
    }

    return num_of_directory_file_operands;
}

char **file_operands_get(const int num_of_arguments, const char **arguments)
{
    if (num_of_arguments == 0 || *arguments == NULL)
        return get_default_file_operands();

    return get_file_operands_from_arguments(num_of_arguments, arguments);
}

unsigned int *file_operands_get_types(char **file_operands)
{
    const unsigned int num_of_operands = ft_str_matrix_length(file_operands);
    unsigned int *file_operands_types = ft_safe_calloc(num_of_operands + 1, sizeof(int));
    struct stat file_stats;

    for (unsigned int i = 0; i < num_of_operands; i++)
    {
        file_stats_get(file_operands[i], &file_stats);

        file_operands_types[i] = file_stats.st_mode & S_IFMT;
    }

    return file_operands_types;
}

char **file_operands_get_non_directory(char **file_operands, const unsigned int *file_operands_types)
{
    const unsigned int num_of_non_directory_file_operands = get_num_of_non_directory_file_operands(file_operands_types);
    char **non_directory_file_operands = ft_safe_calloc(num_of_non_directory_file_operands + 1, sizeof(char *));

    int j = 0;
    for (int i = 0; file_operands[i] != NULL; i++)
    {
        if (!S_ISDIR(file_operands_types[i]))
        {
            non_directory_file_operands[j] = ft_safe_strdup(file_operands[i]);
            j++;
        }
    }

    return non_directory_file_operands;
}

char **file_operands_get_directory(char **file_operands, const unsigned int *file_operand_types)
{
    const unsigned int num_of_directory_file_operands = get_num_of_directory_file_operands(file_operand_types);
    char **directory_file_operands = ft_safe_calloc(num_of_directory_file_operands + 1, sizeof(char *));

    int j = 0;
    for (int i = 0; file_operands[i] != NULL; i++)
    {
        if (S_ISDIR(file_operand_types[i]))
        {
            directory_file_operands[j] = ft_safe_strdup(file_operands[i]);
            j++;
        }
    }

    return directory_file_operands;
}
