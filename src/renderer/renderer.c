#include <stdlib.h>
#include "libft.h"
#include "renderer.h"
#include "render_strategies.h"

typedef void (*t_directory_entries_prefix_strategy)(const t_file_entry_array *file_entry_array);
typedef void (*t_render_entries_strategy)(const t_file_entry_array *file_entry_array);

typedef struct s_render_strategy
{
    t_directory_entries_prefix_strategy render_directory_prefix;
    t_render_entries_strategy render_entries;
} t_render_strategy;

struct s_render_context
{
    int is_first_directory;
    int print_types_separator;
    int print_directory_header;
    const t_render_strategy *strategy;
};

static const t_render_strategy SINGLE_COLUMN_RENDER_STRATEGY = {
    .render_directory_prefix = directory_entries_prefix_single_column_strategy,
    .render_entries = single_column_strategy
};

static const t_render_strategy LONG_FORMAT_RENDER_STRATEGY = {
    .render_directory_prefix = directory_entries_prefix_long_format_strategy,
    .render_entries = long_format_strategy
};

static int should_print_directory_header(const t_parsed_arguments *parsed_arguments)
{
    const int has_multiple_file_operands = parsed_arguments_has_multiple_file_operands(parsed_arguments);
    const int has_directory_file_operands = parsed_arguments_has_directory_file_operands(parsed_arguments);
    const int is_recursive = parsed_arguments_is_option_active(parsed_arguments, OPTIONS_RECURSIVE);

    return (has_multiple_file_operands && has_directory_file_operands) || is_recursive;
}

static const t_render_strategy *select_render_strategy(const t_parsed_arguments *parsed_arguments)
{
    if (parsed_arguments_is_option_active(parsed_arguments, OPTIONS_LONG_FORMAT))
        return &LONG_FORMAT_RENDER_STRATEGY;

    return &SINGLE_COLUMN_RENDER_STRATEGY;
}

static void render_directory_header(t_render_context *context, const char *directory_header)
{
    if (!context->print_directory_header)
        return;

    if (!context->is_first_directory)
        ft_printf("\n");
    ft_printf("%s:\n", directory_header);
    context->is_first_directory = 0;
}

t_render_context *render_context_create(const t_parsed_arguments *parsed_arguments)
{
    if (parsed_arguments == NULL)
        return NULL;

    t_render_context *context = ft_safe_calloc(1, sizeof(t_render_context));

    context->is_first_directory = 1;
    context->print_directory_header = should_print_directory_header(parsed_arguments);
    context->print_types_separator = parsed_arguments_has_mixed_types_file_operands(parsed_arguments);
    context->strategy = select_render_strategy(parsed_arguments);

    return context;
}

void render_context_destroy(t_render_context **context)
{
    if (context == NULL || *context == NULL)
        return;

    free(*context);
    *context = NULL;
}

void render_entries(const t_render_context *context, const t_file_entry_array *file_entry_array)
{
    if (context == NULL || file_entry_array == NULL)
        return;

    context->strategy->render_entries(file_entry_array);
}

void render_directory(t_render_context *context, const char *directory_header, const t_file_entry_array *file_entry_array)
{
    if (context == NULL || !ft_is_valid_path(directory_header) || file_entry_array == NULL)
        return;

    render_directory_header(context, directory_header);
    context->strategy->render_directory_prefix(file_entry_array);
    render_entries(context, file_entry_array);
}

void render_types_separator(const t_render_context *context)
{
    if (context == NULL || !context->print_types_separator)
        return;

    ft_printf("\n");
}
