#include <stdlib.h>
#include "result.h"
#include "libft.h"

struct s_result
{
    void *value;
    int has_failed;
};

t_result *result_create_successful(void *value)
{
    t_result *result = ft_safe_calloc(1, sizeof(t_result));

    result->value = value;
    result->has_failed = 0;

    return result;
}

t_result *result_create_failed(void)
{
    t_result *result = ft_safe_calloc(1, sizeof(t_result));

    result->value = NULL;
    result->has_failed = 1;

    return result;
}

void *result_get_value(const t_result *result)
{
    return result->value;
}

int result_has_failed(const t_result *result)
{
    return result->has_failed;
}

int result_has_succeed(const t_result *result)
{
    return !result->has_failed;
}

void result_destroy(t_result **result)
{
    free(*result);
    *result = NULL;
}
