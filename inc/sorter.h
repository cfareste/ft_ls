#pragma once

typedef struct s_parsed_arguments t_parsed_arguments;
typedef struct s_sorter_context t_sorter_context;

typedef int (*t_comparator_fn)(const void *first_item, const void *second_item);

t_sorter_context *sorter_context_create(const t_parsed_arguments *parsed_arguments);
int sorter_context_is_modification_time_order_active(const t_sorter_context *context);
void sorter_context_destroy(t_sorter_context **context);
void sort_pointer_array(const t_sorter_context *context, void **array, t_comparator_fn compare_fn);
