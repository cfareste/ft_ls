#pragma once

#include "file_entry.h"
#include "parsed_arguments.h"

void directory_entries_prefix_single_column_strategy(const t_parsed_arguments *parsed_arguments, const t_file_entry_array *file_entry_array);
void single_column_strategy(const t_parsed_arguments *parsed_arguments, const t_file_entry_array *file_entry_array);
void multi_column_strategy(const t_parsed_arguments *parsed_arguments, const t_file_entry_array *file_entry_array);
void directory_entries_prefix_long_format_strategy(const t_parsed_arguments *parsed_arguments, const t_file_entry_array *file_entry_array);
void long_format_strategy(const t_parsed_arguments *parsed_arguments, const t_file_entry_array *file_entry_array);
