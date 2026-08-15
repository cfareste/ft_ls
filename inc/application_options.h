#pragma once

typedef struct s_ft_ls_options t_ft_ls_options;

t_ft_ls_options *ft_ls_options_get(const char **arguments);
void ft_ls_options_destroy(t_ft_ls_options **options);
