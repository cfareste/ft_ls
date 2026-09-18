#pragma once

#include "parsed_arguments.h"

#define FT_LS_APPLICATION_SUCCESS (0)
#define FT_LS_APPLICATION_MINOR_ERROR (1)
#define FT_LS_APPLICATION_MAJOR_ERROR (2)

int application_run(const t_result *parsing_arguments_result);
