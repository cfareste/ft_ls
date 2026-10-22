#pragma once

#include "file_stats.h"

char **file_operands_get(int num_of_file_operands, const char **arguments);
t_file_stats **file_operands_get_stats(int num_of_file_operands, char **file_operands);
char **file_operands_get_non_directory(char **file_operands, t_file_stats **file_operands_stats, int has_implicit_default_operand);
char **file_operands_get_directory(char **file_operands, t_file_stats **file_operands_stats, int has_implicit_default_operand);
