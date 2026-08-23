#pragma once

#include "file_entry_list.h"

typedef struct s_render_context t_render_context;

t_render_context *render_context_create();
void render_context_set_directory_header(t_render_context *context, const char *directory_header);
void render_context_destroy(t_render_context **context);

void render(const t_file_entry_list *file_entry_list, t_render_context *context);
