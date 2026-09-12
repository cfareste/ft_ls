#pragma once

typedef struct s_result t_result;

t_result *result_create_successful(void *value);
t_result *result_create_failed(void);
void *result_get_value(const t_result *result);
int result_has_failed(const t_result *result);
void result_destroy(t_result **result);
