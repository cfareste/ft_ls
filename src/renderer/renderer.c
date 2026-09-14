#include <stdlib.h>
#include "libft.h"
#include "renderer.h"

struct s_render_context
{
    int is_first_directory_render;
    int should_print_types_separator;
    int should_print_directory_header;
};

static int check_if_should_print_directory_header(const t_parsed_arguments *parsed_arguments)
{
    const int has_multiple_file_operands = parsed_arguments_has_multiple_file_operands(parsed_arguments);
    const int has_directory_file_operands = parsed_arguments_has_directory_file_operands(parsed_arguments);

    return has_multiple_file_operands && has_directory_file_operands;
}

static void print_directory_header(const t_render_context *context, const char *directory_header)
{
    if (!context->is_first_directory_render)
        ft_printf("\n");

    ft_printf("%s:\n", directory_header);
}

t_render_context *render_context_create(const t_parsed_arguments *parsed_arguments)
{
    if (parsed_arguments == NULL)
        return NULL;

    t_render_context *context = ft_safe_calloc(1, sizeof(t_render_context));

    context->is_first_directory_render = 1;
    context->should_print_directory_header = check_if_should_print_directory_header(parsed_arguments);
    context->should_print_types_separator = parsed_arguments_has_mixed_types_file_operands(parsed_arguments);

    return context;
}

void render_context_destroy(t_render_context **context)
{
    if (context == NULL || *context == NULL)
        return ;

    free(*context);
    *context = NULL;
}

void render(const t_file_entry_array *file_entry_array, const t_render_context *context)
{
    if (file_entry_array == NULL || context == NULL)
        return;

    const unsigned int count = file_entry_array_get_length(file_entry_array);
    for (unsigned int i = 0; i < count; i++)
    {
        const t_file_entry *file_entry = file_entry_array_get_at(file_entry_array, i);
        ft_printf("%s\n", file_entry_get_name(file_entry));
    }
}

void render_types_separator(const t_render_context *context)
{
    if (context == NULL || !context->should_print_types_separator)
        return ;

    ft_printf("\n");
}

static void render_directory_header(t_render_context *context, const char *directory_header)
{
    if (context == NULL || !context->should_print_directory_header || directory_header == NULL)
        return ;

    print_directory_header(context, directory_header);
    context->is_first_directory_render = 0;
}

void render_directory(t_render_context *context, const char *directory_header, const t_file_entry_array *file_entry_array)
{
    render_directory_header(context, directory_header);
    render(file_entry_array, context);
}
