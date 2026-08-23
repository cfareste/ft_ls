#pragma once

typedef struct s_parsed_arguments t_parsed_arguments;

t_parsed_arguments *parse_arguments(int num_of_arguments, const char **arguments);
const char * const *parsed_arguments_get_file_operands(const t_parsed_arguments *parsed_arguments);
const char * const *parsed_arguments_get_non_directory_file_operands(const t_parsed_arguments *parsed_arguments);
const char * const *parsed_arguments_get_directory_file_operands(const t_parsed_arguments *parsed_arguments);
int parsed_arguments_has_multiple_file_operands(const t_parsed_arguments *parsed_arguments);
int parsed_arguments_has_directory_file_operands(const t_parsed_arguments *parsed_arguments);
void parsed_arguments_destroy(t_parsed_arguments **parsed_arguments);
