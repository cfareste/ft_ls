#include <stdio.h>
#include <stdlib.h>

#include "mocks.h"

t_failure_type g_opendir_fail = NO_FAILURE;

DIR *opendir_adapter(const char *path)
{
    if (g_opendir_fail != NO_FAILURE)
    {
        return NULL;
    }

    printf("DIDNT FAIL");
    (void) path;

    return NULL;
}
    