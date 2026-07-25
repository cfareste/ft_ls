#include <stdlib.h>

#include "libft.h"
#include "mocks.h"

typedef struct DIR {} DIR;

t_failure_type g_opendir_fail = NO_FAILURE;

DIR *opendir_adapter(const char *path)
{
    (void) path;
    if (g_opendir_fail != NO_FAILURE)
    {
        return NULL;
    }

    DIR *dir = ft_calloc(1, sizeof(DIR));
    return dir;
}
