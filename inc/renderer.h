#pragma once

#include "file_entry_list.h"

typedef struct s_render_context t_render_context;

t_render_context *render_context_create();
t_render_context *render_context_create_for_directory_file_operands(const char *directory_header);
void render_context_set_should_print_directory_header(t_render_context *config, int should_print_directory_header);
void render_context_set_should_print_directory_header_leading_newline(t_render_context *config, int should_print_directory_header_leading_newline);
void render_context_destroy(t_render_context **config);

void render(const t_file_entry_list *file_entry_list, const t_render_context *config);
