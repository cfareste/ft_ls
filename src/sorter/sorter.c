#include <stdlib.h>
#include "sorter.h"
#include "parsed_arguments.h"
#include "libft.h"

static void merge_ranges(void **array, void **buffer, const size_t count, const size_t middle,
    const t_comparator_fn compare_fn, const int reverse_order)
{
    size_t left = 0;
    size_t right = middle;
    size_t output = 0;

    while (left < middle && right < count)
    {
        int comparison = compare_fn(array[left], array[right]);

        if (reverse_order)
            comparison = -comparison;

        if (comparison <= 0)
            buffer[output++] = array[left++];
        else
            buffer[output++] = array[right++];
    }

    while (left < middle)
        buffer[output++] = array[left++];
    while (right < count)
        buffer[output++] = array[right++];

    ft_memcpy(array, buffer, count * sizeof(*array));
}

static void merge_sort(void **array, void **buffer, const size_t count,
    const t_comparator_fn compare_fn, const int reverse_order)
{
    if (count < 2)
        return;

    const size_t middle = count / 2;
    merge_sort(array, buffer, middle, compare_fn, reverse_order);
    merge_sort(array + middle, buffer, count - middle, compare_fn, reverse_order);
    merge_ranges(array, buffer, count, middle, compare_fn, reverse_order);
}

void sort_pointer_array(const t_parsed_arguments *parsed_arguments, void **array, const t_comparator_fn compare_fn)
{
    if (array == NULL || *array == NULL || compare_fn == NULL)
        return;

    if (parsed_arguments_get_sort_type(parsed_arguments) == SORT_NONE)
        return;

    size_t count = 0;
    while (array[count] != NULL)
        count++;
    if (count < 2)
        return;

    void **buffer = ft_safe_calloc(count, sizeof(*buffer));
    merge_sort(array, buffer, count, compare_fn, parsed_arguments_should_reverse_sort(parsed_arguments));
    free(buffer);
}
