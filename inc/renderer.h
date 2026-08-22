#pragma once

#include "file_entry_list.h"

typedef struct s_render_config t_render_config;

t_render_config *render_config_create_for_non_directory_file_operands();
void render_config_destroy(t_render_config **config);

void render(const t_file_entry_list *file_entry_list);
