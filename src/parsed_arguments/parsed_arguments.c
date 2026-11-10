#include <stdlib.h>
#include "parsed_arguments.h"
#include "file_operands.h"
#include "sorter.h"
#include "error_reporter.h"
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

typedef struct s_file_operand_sort_entry
{
    char *name;
    t_file_stats *stats;
} t_file_operand_sort_entry;

static int compare_by_name(const void *first_str, const void *second_str)
{
    return ft_strcmp(first_str, second_str);
}

static int compare_file_operands_by_modification_time(const void *first_item, const void *second_item)
{
    const t_file_operand_sort_entry *first = first_item;
    const t_file_operand_sort_entry *second = second_item;
    const time_t first_timestamp = file_stats_get_modification_date(first->stats).timestamp;
    const time_t second_timestamp = file_stats_get_modification_date(second->stats).timestamp;

    if (first_timestamp > second_timestamp)
        return -1;
    if (first_timestamp < second_timestamp)
        return 1;
    return ft_strcmp(first->name, second->name);
}

static t_file_stats **get_file_operand_stats(char **operands, char **file_operands, t_file_stats **file_operands_stats)
{
    unsigned int count = 0;

    while (operands[count] != NULL)
        count++;

    t_file_stats **operands_stats = ft_safe_calloc(count + 1, sizeof(t_file_stats *));

    for (unsigned int i = 0; i < count; i++)
    {
        for (unsigned int j = 0; file_operands[j] != NULL; j++)
        {
            if (ft_are_string_equals(operands[i], file_operands[j]))
            {
                operands_stats[i] = file_operands_stats[j];
                break;
            }
        }
    }
    return operands_stats;
}

static void sort_file_operands_by_modification_time(
    const t_sorter_context *sorter_context,
    char **operands,
    t_file_stats **operands_stats)
{
    unsigned int count = 0;

    while (operands[count] != NULL)
        count++;

    t_file_operand_sort_entry *entries = ft_safe_calloc(count, sizeof(t_file_operand_sort_entry));
    void **entries_to_sort = ft_safe_calloc(count + 1, sizeof(void *));
    for (unsigned int i = 0; i < count; i++)
    {
        entries[i].name = operands[i];
        entries[i].stats = operands_stats[i];
        entries_to_sort[i] = &entries[i];
    }

    sort_pointer_array(sorter_context, entries_to_sort, compare_file_operands_by_modification_time);

    for (unsigned int i = 0; i < count; i++)
    {
        const t_file_operand_sort_entry *entry = entries_to_sort[i];
        operands[i] = entry->name;
        operands_stats[i] = entry->stats;
    }
    free(entries_to_sort);
    free(entries);
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

static const char *retrieve_advanced_file_stats(char **file_operands, t_file_stats **file_operands_stats)
{
    const char *failed_operand = NULL;

    for (unsigned int i = 0; file_operands[i] != NULL; i++)
    {
        if (file_operands_stats[i] == NULL)
            continue;

        file_stats_get_author(file_operands_stats[i]);
        file_stats_get_group(file_operands_stats[i]);
        file_stats_get_target_pointed_by_link(file_operands_stats[i]);
        if (failed_operand == NULL && file_stats_target_retrieval_failed(file_operands_stats[i]))
            failed_operand = file_operands[i];
    }
    return failed_operand;
}

t_result *parse_arguments(const int num_of_arguments, const char **arguments)
{
    if (num_of_arguments < 0 || arguments == NULL)
        return NULL;

    const char invalid_option = options_get_invalid_option(arguments);
    if (invalid_option != '\0')
    {
        report_invalid_option(invalid_option);
        return result_create_failed(NULL, NULL);
    }

    t_parsed_arguments *parsed_arguments = ft_safe_calloc(1, sizeof(t_parsed_arguments));
    const int num_of_file_operands = num_of_arguments - options_get_amount(arguments);

    parsed_arguments->options = options_get(arguments);
    parsed_arguments->has_implicit_default_operand = num_of_file_operands == 0;
    parsed_arguments->file_operands = file_operands_get(num_of_file_operands, arguments);
    parsed_arguments->file_operands_stats = file_operands_get_stats(num_of_file_operands, parsed_arguments->file_operands, !parsed_arguments_is_long_format_active(parsed_arguments));
    const char *failed_advanced_metadata_operand = NULL;
    if (parsed_arguments_is_long_format_active(parsed_arguments))
        failed_advanced_metadata_operand = retrieve_advanced_file_stats(parsed_arguments->file_operands, parsed_arguments->file_operands_stats);
    parsed_arguments->non_directory_file_operands = file_operands_get_non_directory(parsed_arguments->file_operands, parsed_arguments->file_operands_stats, parsed_arguments->has_implicit_default_operand);
    parsed_arguments->directory_file_operands = file_operands_get_directory(parsed_arguments->file_operands, parsed_arguments->file_operands_stats, parsed_arguments->has_implicit_default_operand);
    t_sorter_context *sorter_context = sorter_context_create(parsed_arguments);
    if (parsed_arguments_is_option_active(parsed_arguments, OPTIONS_TIME_SORTING))
    {
        parsed_arguments->non_directory_file_operands_stats = get_file_operand_stats(parsed_arguments->non_directory_file_operands, parsed_arguments->file_operands, parsed_arguments->file_operands_stats);
        t_file_stats **directory_file_operands_stats = get_file_operand_stats(parsed_arguments->directory_file_operands, parsed_arguments->file_operands, parsed_arguments->file_operands_stats);
        sort_file_operands_by_modification_time(sorter_context, parsed_arguments->non_directory_file_operands, parsed_arguments->non_directory_file_operands_stats);
        sort_file_operands_by_modification_time(sorter_context, parsed_arguments->directory_file_operands, directory_file_operands_stats);
        free(directory_file_operands_stats);
    }
    else
    {
        sort_pointer_array(sorter_context, (void **) parsed_arguments->non_directory_file_operands, compare_by_name);
        sort_pointer_array(sorter_context, (void **) parsed_arguments->directory_file_operands, compare_by_name);
    }
    sorter_context_destroy(&sorter_context);
    if (parsed_arguments->non_directory_file_operands_stats == NULL)
        parsed_arguments->non_directory_file_operands_stats = get_file_operand_stats(parsed_arguments->non_directory_file_operands, parsed_arguments->file_operands, parsed_arguments->file_operands_stats);

    if (failed_advanced_metadata_operand != NULL)
        return result_create_failed(parsed_arguments, failed_advanced_metadata_operand);
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
