#include <stdlib.h>

#include "libft.h"
#include "mocks.h"

typedef struct DIR {} DIR;
struct dirent {};

t_failure_type g_opendir_fail = NO_FAILURE;
t_failure_type g_readdir_fail = NO_FAILURE;

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

struct dirent *readdir_adapter(DIR *dir)
{
    (void) dir;
    if (g_readdir_fail != NO_FAILURE)
    {
        return NULL;
    }

    struct dirent *dirent = ft_calloc(1, sizeof(struct dirent));
    return dirent;
}
