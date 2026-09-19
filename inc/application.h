#pragma once

#include "parsed_arguments.h"

typedef enum e_ft_ls_application_status
{
    FT_LS_APPLICATION_SUCCESS,
    FT_LS_APPLICATION_MINOR_ERROR,
    FT_LS_APPLICATION_MAJOR_ERROR
} t_ft_ls_application_status;

t_ft_ls_application_status application_run(const t_result *parsing_arguments_result);
