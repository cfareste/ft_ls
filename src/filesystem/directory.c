#include <dirent.h>
#include <stdlib.h>
#include "libft.h"
#include "directory.h"

static int is_valid_path(const char *path)
{
    return path != NULL && !ft_is_str_empty(path);
}

DIR *directory_open(const char *path)
{
    if (!is_valid_path(path))
        return NULL;

    return opendir(path);
}

struct dirent *directory_get_next_entry(DIR *dir)
{
    if (dir == NULL)
        return NULL;

    return readdir(dir);
}

int directory_close(DIR **dir_stream)
{
    if (dir_stream == NULL || *dir_stream == NULL)
        return -1;

    const int result = closedir(*dir_stream);
    *dir_stream = NULL;

    return result;
}
