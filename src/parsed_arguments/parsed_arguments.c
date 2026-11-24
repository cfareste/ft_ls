#include <stdlib.h>
#include <unistd.h>
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
    int stdout_is_terminal;
    char **non_directory_file_operands;
    char **directory_file_operands;
};

typedef struct s_file_operand_sort_entry
{
    char *name;
    t_file_stats *stats;
} t_file_operand_sort_entry;

static int compare_file_operand_entries_by_name(const void *first_item, const void *second_item)
{
    const t_file_operand_sort_entry *first = first_item;
    const t_file_operand_sort_entry *second = second_item;

    return ft_strcmp(first->name, second->name);
}

static int compare_file_operand_entries_by_modification_time(const void *first_item, const void *second_item)
{
    const t_file_operand_sort_entry *first = first_item;
    const t_file_operand_sort_entry *second = second_item;
    const time_t first_timestamp = file_stats_get_modification_date(first->stats).timestamp;
    const time_t second_timestamp = file_stats_get_modification_date(second->stats).timestamp;

    if (first_timestamp > second_timestamp)
        return -1;
    if (first_timestamp < second_timestamp)
        return 1;
    return compare_file_operand_entries_by_name(first, second);
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
        file_stats_get_acl_type(file_operands_stats[i]);
        if (failed_operand == NULL
            && (file_stats_target_retrieval_failed(file_operands_stats[i])
                || file_stats_acl_retrieval_failed(file_operands_stats[i])))
            failed_operand = file_operands[i];
    }
    return failed_operand;
}

static const char *initialize_file_operands(t_parsed_arguments *parsed_arguments, const int num_of_arguments, const char **arguments)
{
    const int num_of_file_operands = num_of_arguments - options_get_amount(arguments);

    parsed_arguments->has_implicit_default_operand = num_of_file_operands == 0;
    parsed_arguments->file_operands = file_operands_get(num_of_file_operands, arguments);
    parsed_arguments->file_operands_stats = file_operands_get_stats(num_of_file_operands, parsed_arguments->file_operands, !parsed_arguments_is_long_format_active(parsed_arguments));

    if (parsed_arguments_is_long_format_active(parsed_arguments))
        return retrieve_advanced_file_stats(parsed_arguments->file_operands, parsed_arguments->file_operands_stats);

    return NULL;
}

static t_file_stats **get_operand_group_stats(const t_parsed_arguments *parsed_arguments, char **group_operands)
{
    const int count = ft_str_matrix_length(group_operands);

    t_file_stats **group_stats = ft_safe_calloc(count + 1, sizeof(t_file_stats *));

    for (int group_index = 0; group_index < count; group_index++)
    {
        const char *name = group_operands[group_index];
        for (int source_index = 0; parsed_arguments->file_operands[source_index] != NULL; source_index++)
        {
            if (ft_are_string_equals(name, parsed_arguments->file_operands[source_index]))
            {
                group_stats[group_index] = parsed_arguments->file_operands_stats[source_index];
                break;
            }
        }
    }
    return group_stats;
}

static void sort_file_operand_group(char **operands, t_file_stats **operands_stats,
    const t_sorter_context *sorter_context, const t_comparator_fn comparator)
{
    const int count = ft_str_matrix_length(operands);
    if (count < 2)
        return;

    t_file_operand_sort_entry *entries = ft_safe_calloc(count, sizeof(t_file_operand_sort_entry));
    void **entries_to_sort = ft_safe_calloc(count + 1, sizeof(void *));
    for (int i = 0; i < count; i++)
    {
        entries[i].name = operands[i];
        entries[i].stats = operands_stats[i];
        entries_to_sort[i] = &entries[i];
    }

    sort_pointer_array(sorter_context, entries_to_sort, comparator);

    for (int i = 0; i < count; i++)
    {
        const t_file_operand_sort_entry *entry = entries_to_sort[i];
        operands[i] = entry->name;
        operands_stats[i] = entry->stats;
    }
    free(entries_to_sort);
    free(entries);
}

static void prepare_operand_groups(t_parsed_arguments *parsed_arguments)
{
    char **operands = parsed_arguments->file_operands;
    t_file_stats **stats = parsed_arguments->file_operands_stats;
    const int has_default_operand = parsed_arguments->has_implicit_default_operand;
    char **files = file_operands_get_non_directory(operands, stats, has_default_operand);
    char **directories = file_operands_get_directory(operands, stats, has_default_operand);
    t_file_stats **file_stats = get_operand_group_stats(parsed_arguments, files);

    parsed_arguments->non_directory_file_operands = files;
    parsed_arguments->non_directory_file_operands_stats = file_stats;
    parsed_arguments->directory_file_operands = directories;

    if (parsed_arguments_is_option_active(parsed_arguments, OPTIONS_NO_SORT))
        return;

    t_sorter_context *sorter_context = sorter_context_create(parsed_arguments);
    const t_comparator_fn comparator = sorter_context_is_modification_time_order_active(sorter_context)
        ? compare_file_operand_entries_by_modification_time
        : compare_file_operand_entries_by_name;
    t_file_stats **directory_stats = get_operand_group_stats(parsed_arguments, directories);

    sort_file_operand_group(files, file_stats, sorter_context, comparator);
    sort_file_operand_group(directories, directory_stats, sorter_context, comparator);

    free(directory_stats);
    sorter_context_destroy(&sorter_context);
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

    parsed_arguments->stdout_is_terminal = isatty(STDOUT_FILENO);
    parsed_arguments->options = options_get(arguments);
    const char *failed_advanced_metadata_operand = initialize_file_operands(parsed_arguments, num_of_arguments, arguments);
    prepare_operand_groups(parsed_arguments);

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

int parsed_arguments_is_stdout_a_tty(const t_parsed_arguments *parsed_arguments)
{
    if (parsed_arguments == NULL)
        return 0;

    return parsed_arguments->stdout_is_terminal;
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
