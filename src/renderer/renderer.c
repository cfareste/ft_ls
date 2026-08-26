#include <stdlib.h>
#include "libft.h"
#include "renderer.h"

struct s_render_context
{
    char *directory_header;
    int is_first_render;
};

static void print_directory_header(const t_render_context *context)
{
    if (!context->is_first_render)
        ft_printf("\n");

    ft_printf("%s:\n", context->directory_header);
}

t_render_context *render_context_create()
{
    t_render_context *config = ft_safe_calloc(1, sizeof(t_render_context));

    config->directory_header = NULL;
    config->is_first_render = 1;

    return config;
}

void render_context_set_directory_header(t_render_context *context, const char *directory_header)
{
    if (context == NULL || directory_header == NULL)
        return;

    free(context->directory_header);
    context->directory_header = ft_safe_strdup(directory_header);
}

void render_context_destroy(t_render_context **context)
{
    if (context == NULL || *context == NULL)
        return ;

    free((*context)->directory_header);
    free(*context);
    *context = NULL;
}

void render(const t_file_entry_list *file_entry_list, t_render_context *context)
{
    if (context->directory_header != NULL)
        print_directory_header(context);

    const t_file_entry_list *entry = file_entry_list;
    while (entry != NULL)
    {
        ft_printf("%s\n", file_entry_list_get_name(entry));
        entry = file_entry_list_get_next(entry);
    }

    context->is_first_render = 0;
}
