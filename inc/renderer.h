#pragma once

#include "file_entry.h"
#include "parsed_arguments.h"

typedef struct s_render_context t_render_context;

t_render_context *render_context_create(const t_parsed_arguments *parsed_arguments);
void render_context_destroy(t_render_context **context);
void render_entries(const t_file_entry_array *file_entry_array, const t_render_context *context);
void render_directory(t_render_context *context, const char *directory_header, const t_file_entry_array *file_entry_array);
void render_types_separator(const t_render_context *context);
