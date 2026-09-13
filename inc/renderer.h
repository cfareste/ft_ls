#pragma once

#include "file_entry.h"
#include "parsed_arguments.h"

typedef struct s_render_context t_render_context;

t_render_context *render_context_create(const t_parsed_arguments *parsed_arguments);
void render_context_set_directory_header(t_render_context *context, const char *directory_header);
void render_context_destroy(t_render_context **context);
void render(const t_file_entry_array *file_entry_array, t_render_context *context);
void render_types_separator(t_render_context *context);
