#pragma once

#include "file_entry_list.h"

typedef struct s_render_config t_render_config;

t_render_config *render_config_create_for_non_directory_file_operands();
t_render_config *render_config_create_for_directory_file_operands(const char *directory_header);
void render_config_set_should_print_directory_header(t_render_config *config, int should_print_directory_header);
void render_config_destroy(t_render_config **config);

void render(const t_file_entry_list *file_entry_list, const t_render_config *config);
