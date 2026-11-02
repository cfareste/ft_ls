#include <stddef.h>
#include <stdlib.h>
#include "sorter.h"
#include "libft.h"

struct s_sorter_context
{
    const t_parsed_arguments *parsed_arguments;
};

t_sorter_context *sorter_context_create(const t_parsed_arguments *parsed_arguments)
{
    t_sorter_context *context = ft_safe_calloc(1, sizeof(t_sorter_context));

    context->parsed_arguments = parsed_arguments;
    return context;
}

void sorter_context_destroy(t_sorter_context **context)
{
    if (context == NULL || *context == NULL)
        return ;

    free(*context);
    *context = NULL;
}

void sort_pointer_array(const t_sorter_context *context, void **array, const t_comparator_fn compare_fn)
{
    (void) context;
    if (array == NULL || *array == NULL || compare_fn == NULL)
        return;

    for (unsigned int i = 0; array[i] != NULL; i++)
    {
        for (unsigned int j = i + 1; array[j] != NULL; j++)
        {
            if (compare_fn(array[i], array[j]) <= 0)
                continue ;

            void *tmp = array[i];
            array[i] = array[j];
            array[j] = tmp;
        }
    }
}
