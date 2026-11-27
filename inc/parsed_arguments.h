#pragma once

#include "result.h"
#include "option_types.h"
#include "file_stats.h"

typedef struct s_parsed_arguments t_parsed_arguments;

t_result *parse_arguments(int num_of_arguments, const char **arguments);
const char * const *parsed_arguments_get_non_directory_file_operands(const t_parsed_arguments *parsed_arguments);
t_file_stats * const *parsed_arguments_get_non_directory_file_operands_stats(const t_parsed_arguments *parsed_arguments);
const char * const *parsed_arguments_get_directory_file_operands(const t_parsed_arguments *parsed_arguments);
t_format parsed_arguments_get_format(const t_parsed_arguments *parsed_arguments);
t_sort_type parsed_arguments_get_sort_type(const t_parsed_arguments *parsed_arguments);
t_time_type parsed_arguments_get_time_type(const t_parsed_arguments *parsed_arguments);
int parsed_arguments_should_print_owner(const t_parsed_arguments *parsed_arguments);
int parsed_arguments_should_reverse_sort(const t_parsed_arguments *parsed_arguments);
int parsed_arguments_should_filter_hidden_files(const t_parsed_arguments *parsed_arguments);
int parsed_arguments_should_recurse(const t_parsed_arguments *parsed_arguments);
int parsed_arguments_should_color_files(const t_parsed_arguments *parsed_arguments);
int parsed_arguments_is_stdout_a_tty(const t_parsed_arguments *parsed_arguments);
int parsed_arguments_is_file_operand(const t_parsed_arguments *parsed_arguments, const char *file);
int parsed_arguments_has_multiple_file_operands(const t_parsed_arguments *parsed_arguments);
int parsed_arguments_has_directory_file_operands(const t_parsed_arguments *parsed_arguments);
int parsed_arguments_has_mixed_types_file_operands(const t_parsed_arguments *parsed_arguments);
void parsed_arguments_destroy(t_parsed_arguments **parsed_arguments);
