#pragma once

typedef struct s_parsed_arguments t_parsed_arguments;

typedef int (*t_comparator_fn)(const void *first_item, const void *second_item);

void sort_pointer_array(const t_parsed_arguments *parsed_arguments, void **array, t_comparator_fn compare_fn);
