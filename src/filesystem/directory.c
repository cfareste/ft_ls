#include <dirent.h>
#include <stdlib.h>
#include "libft.h"
#include "directory.h"


static int is_invalid_path(const char *path)
{
    return path == NULL || path[0] == '\0';
}

DIR *directory_open(const char *path)
{
    if (is_invalid_path(path))
        return NULL;

    return opendir(path);
}

struct dirent *directory_get_next_entry(DIR *dir)
{
    return readdir(dir);
}

int directory_close(DIR *dir_stream)
{
    if (dir_stream == NULL)
        return 0;

    const int result = closedir(dir_stream);

    return result;
}
