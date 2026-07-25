#include <stdlib.h>
#include <string.h>

#include "mocks.h"

t_failure_type g_opendir_fail = NO_FAILURE;
t_failure_type g_readdir_fail = NO_FAILURE;
static char g_dirent_name[256] = "";

DIR *opendir_adapter(const char *path)
{
    (void) path;
    if (g_opendir_fail != NO_FAILURE)
    {
        return NULL;
    }

    DIR *dir = calloc(1, sizeof(DIR));
    return dir;
}

struct dirent *readdir_adapter(DIR *dir)
{
    (void) dir;
    if (g_readdir_fail != NO_FAILURE)
    {
        return NULL;
    }

    struct dirent *dirent = calloc(1, sizeof(struct dirent));
    strlcpy(dirent->d_name, g_dirent_name, sizeof(dirent->d_name));

    return dirent;
}

void affirm_readdir_will_return_a_file_named(const char *path)
{
    strlcpy(g_dirent_name, path, sizeof(g_dirent_name));
}
