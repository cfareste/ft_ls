#pragma once

char **get_file_operands(int num_of_arguments, const char **arguments);
unsigned int *get_file_operand_types(char **file_operands);
char **file_operands_get_non_directory(char **file_operands, const unsigned int *file_operands_types);
char **file_operands_get_directory(char **file_operands, const unsigned int *file_operand_types);
