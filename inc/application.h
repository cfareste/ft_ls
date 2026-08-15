#pragma once

#include "application_options.h"

#define FT_LS_APPLICATION_SUCCESS (0)
#define FT_LS_APPLICATION_MINOR_ERROR (1)
#define FT_LS_APPLICATION_MAJOR_ERROR (2)

int run_application(const t_ft_ls_options * options);
