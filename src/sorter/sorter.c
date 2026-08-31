#include <stddef.h>
#include "sorter.h"

void sort_pointer_array(void **array, const t_comparator_fn compare_fn)
{
    if (array == NULL || *array == NULL)
        return;

    for (unsigned int i = 0; array[i] != NULL; i++)
    {
        for (unsigned int j = i + 1; array[j] != NULL; j++)
        {
            if (compare_fn(array[i], array[j]) < 0)
                continue ;

            void *tmp = array[i];
            array[i] = array[j];
            array[j] = tmp;
        }
    }
}
