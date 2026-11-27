#include "sorter.h"
#include "parsed_arguments.h"

void sort_pointer_array(const t_parsed_arguments *parsed_arguments, void **array, const t_comparator_fn compare_fn)
{
    if (array == NULL || *array == NULL || compare_fn == NULL)
        return;

    if (parsed_arguments_get_sort_type(parsed_arguments) == SORT_NONE)
        return;

    const int reverse_order = parsed_arguments_should_reverse_sort(parsed_arguments);
    for (unsigned int i = 0; array[i] != NULL; i++)
    {
        for (unsigned int j = i + 1; array[j] != NULL; j++)
        {
            const int comparison = compare_fn(array[i], array[j]);
            if ((!reverse_order && comparison <= 0) || (reverse_order && comparison >= 0))
                continue ;

            void *tmp = array[i];
            array[i] = array[j];
            array[j] = tmp;
        }
    }
}
