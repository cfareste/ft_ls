#include <stdlib.h>
#include "result.h"
#include "libft.h"

struct s_result
{
    int i;
};

t_result *result_create_successful(void *value)
{
    t_result *result = ft_safe_calloc(1, sizeof(t_result));

    (void) value;
    return result;
}

void *result_get_value(t_result *result)
{
    (void) result;
    return "value";
}

int result_has_failed(t_result *result)
{
    (void) result;
    return 0;
}

void result_destroy(t_result **result)
{
    free(*result);
}
