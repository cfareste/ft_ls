#include <stdlib.h>
#include "libft.h"
#include "renderer.h"

struct s_render_context
{
    char *directory_header;
    int should_print_directory_header;
    int should_print_directory_header_leading_newline;
};

t_render_context *render_context_create_for_non_directory_file_operands()
{
    t_render_context *config = ft_safe_calloc(1, sizeof(t_render_context));

    config->directory_header = NULL;
    config->should_print_directory_header = 0;
    config->should_print_directory_header_leading_newline = 0;

    return config;
}

t_render_context *render_context_create_for_directory_file_operands(const char *directory_header)
{
    t_render_context *config = ft_safe_calloc(1, sizeof(t_render_context));

    config->directory_header = ft_safe_strdup(directory_header);
    config->should_print_directory_header = 1;
    config->should_print_directory_header_leading_newline = 1;

    return config;
}

void render_context_set_should_print_directory_header(t_render_context *config, const int should_print_directory_header)
{
    config->should_print_directory_header = should_print_directory_header;
}

void render_context_set_should_print_directory_header_leading_newline(t_render_context *config, const int should_print_directory_header_leading_newline)
{
    config->should_print_directory_header_leading_newline = should_print_directory_header_leading_newline;
}

void render_context_destroy(t_render_context **config)
{
    if (config == NULL || *config == NULL)
        return ;

    free((*config)->directory_header);
    free(*config);
    *config = NULL;
}

void render(const t_file_entry_list *file_entry_list, const t_render_context *config)
{
    if (file_entry_list == NULL)
        return;

    if (config->should_print_directory_header)
    {
        if (config->should_print_directory_header_leading_newline)
            ft_printf("\n");
        ft_printf("%s:\n", config->directory_header);
    }

    const t_file_entry_list *entry = file_entry_list;
    while (entry != NULL)
    {
        ft_printf("%s\n", file_entry_list_get_name(entry));
        entry = file_entry_list_get_next(entry);
    }
}
