#pragma once

#include "result.h"
#include "parsed_arguments.h"

typedef struct s_scanner_context t_scanner_context;

t_scanner_context *scanner_context_create(const t_parsed_arguments *parsed_arguments);
void scanner_context_destroy(t_scanner_context **context);
t_result *scan(const t_scanner_context *context, const char *path);
