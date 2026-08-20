#include <stdlib.h>
#include "parsed_arguments.h"
#include "file_stats.h"
#include "libft.h"

#define DEFAULT_NUM_OF_OPERANDS 1
#define DEFAULT_FILE_OPERAND "."

struct s_parsed_arguments
{
    unsigned int num_of_file_operands;
    char **file_operands;
    unsigned int *file_operand_types;
    char **non_directory_file_operands;
};

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

static unsigned int *get_file_operand_types(const t_parsed_arguments *parsed_arguments)
{
    const unsigned int num_of_operands = parsed_arguments->num_of_file_operands;
    unsigned int *file_operands_types = ft_safe_calloc(num_of_operands + 1, sizeof(int));
    struct stat file_stats;

    for (unsigned int i = 0; i < num_of_operands; i++)
    {
        get_file_stats(parsed_arguments->file_operands[i], &file_stats);

        file_operands_types[i] = file_stats.st_mode & S_IFMT;
    }

    return file_operands_types;
}

static char **get_non_directory_file_operands(const t_parsed_arguments *parsed_arguments)
{
    const unsigned int num_of_non_directory_file_operands = get_num_of_non_directory_file_operands(parsed_arguments->file_operand_types);
    char **non_directory_file_operands = ft_safe_calloc(num_of_non_directory_file_operands + 1, sizeof(char *));

    int j = 0;
    for (int i = 0; parsed_arguments->file_operands[i] != NULL; i++)
    {
        if (!S_ISDIR(parsed_arguments->file_operand_types[i]))
        {
            non_directory_file_operands[j] = ft_safe_strdup(parsed_arguments->file_operands[i]);
            j++;
        }
    }

    return non_directory_file_operands;
}

t_parsed_arguments *parse_arguments(const int num_of_arguments, const char **arguments)
{
    if (num_of_arguments < 0 || arguments == NULL)
        return NULL;

    t_parsed_arguments *parsed_arguments = ft_safe_calloc(1, sizeof(t_parsed_arguments));
    parsed_arguments->num_of_file_operands = num_of_arguments;
    parsed_arguments->file_operands = get_file_operands(num_of_arguments, arguments);
    parsed_arguments->file_operand_types = get_file_operand_types(parsed_arguments);
    parsed_arguments->non_directory_file_operands = get_non_directory_file_operands(parsed_arguments);

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
    return (const char * const *) parsed_arguments->non_directory_file_operands;
}

void parsed_arguments_destroy(t_parsed_arguments **parsed_arguments)
{
    if (parsed_arguments == NULL || *parsed_arguments == NULL)
        return ;

    free((*parsed_arguments)->file_operand_types);
    ft_free_str_matrix((*parsed_arguments)->file_operands);
    ft_free_str_matrix((*parsed_arguments)->non_directory_file_operands);
    free(*parsed_arguments);

    *parsed_arguments = NULL;
}
