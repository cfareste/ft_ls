#pragma once

typedef int (*t_comparator_fn)(const void *first_item, const void *second_item);

void sort_pointer_array(void **array, t_comparator_fn compare_fn);
