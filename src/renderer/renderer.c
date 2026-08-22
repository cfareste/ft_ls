#include <stdlib.h>
#include "libft.h"
#include "renderer.h"

struct s_render_config
{

};

t_render_config *render_config_create_for_non_directory_file_operands()
{
    t_render_config *config = ft_safe_calloc(1, sizeof(t_render_config));

    return config;
}

t_render_config *render_config_create_for_directory_file_operands()
{
    t_render_config *config = ft_safe_calloc(1, sizeof(t_render_config));

    return config;
}

void render_config_destroy(t_render_config **config)
{
    if (config == NULL || *config == NULL)
        return ;

    free(*config);
    *config = NULL;
}

void render(const t_file_entry_list *file_entry_list)
{
    if (file_entry_list == NULL)
        return;

    const t_file_entry_list *entry = file_entry_list;
    while (entry != NULL)
    {
        ft_printf("%s\n", file_entry_list_get_name(entry));
        entry = file_entry_list_get_next(entry);
    }
}
