#pragma once

#include "file_stats.h"

char **file_operands_get(int num_of_file_operands, const char **arguments);
t_file_type *file_operands_get_types(int num_of_file_operands, char **file_operands);
char **file_operands_get_non_directory(char **file_operands, const t_file_type *file_operands_types);
char **file_operands_get_directory(char **file_operands, const t_file_type *file_operand_types);
