#pragma once

typedef struct s_result t_result;

t_result *result_create_successful(void *value);
void *result_get_value(t_result *result);
int result_has_failed(t_result *result);
void result_destroy(t_result **result);
