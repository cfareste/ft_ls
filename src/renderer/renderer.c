#include <stdlib.h>
#include "libft.h"
#include "renderer.h"
#include "render_strategies.h"

typedef void (*t_render_strategy)(const t_file_entry_array *file_entry_array);

struct s_render_context
{
    int is_first_directory_render;
    int should_print_types_separator;
    int should_print_directory_header;
    t_render_strategy render_strategy;
};

static int check_if_should_print_directory_header(const t_parsed_arguments *parsed_arguments)
{
    const int has_multiple_file_operands = parsed_arguments_has_multiple_file_operands(parsed_arguments);
    const int has_directory_file_operands = parsed_arguments_has_directory_file_operands(parsed_arguments);
    const int has_recursive_option = parsed_arguments_is_option_active(parsed_arguments, OPTIONS_RECURSIVE);

    return (has_multiple_file_operands && has_directory_file_operands) || has_recursive_option;
}

static void render_directory_header(t_render_context *context, const char *directory_header)
{
    if (!context->should_print_directory_header)
        return ;

    if (!context->is_first_directory_render)
        ft_printf("\n");
    ft_printf("%s:\n", directory_header);

    context->is_first_directory_render = 0;
}

t_render_context *render_context_create(const t_parsed_arguments *parsed_arguments)
{
    if (parsed_arguments == NULL)
        return NULL;

    t_render_context *context = ft_safe_calloc(1, sizeof(t_render_context));

    context->is_first_directory_render = 1;
    context->should_print_directory_header = check_if_should_print_directory_header(parsed_arguments);
    context->should_print_types_separator = parsed_arguments_has_mixed_types_file_operands(parsed_arguments);
    context->render_strategy = single_column_strategy;

    return context;
}

void render_context_destroy(t_render_context **context)
{
    if (context == NULL || *context == NULL)
        return ;

    free(*context);
    *context = NULL;
}

void render_entries(const t_render_context *context, const t_file_entry_array *file_entry_array)
{
    if (context == NULL || file_entry_array == NULL)
        return;

    context->render_strategy(file_entry_array);
}

void render_directory(t_render_context *context, const char *directory_header, const t_file_entry_array *file_entry_array)
{
    if (context == NULL || !ft_is_valid_path(directory_header) || file_entry_array == NULL)
        return ;

    render_directory_header(context, directory_header);
    render_entries(context, file_entry_array);
}

void render_types_separator(const t_render_context *context)
{
    if (context == NULL || !context->should_print_types_separator)
        return ;

    ft_printf("\n");
}
