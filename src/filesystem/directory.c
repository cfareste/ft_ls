#include <dirent.h>
#include <stdlib.h>
#include "libft.h"
#include "directory.h"

DIR *directory_open(const char *path)
{
    if (!ft_is_valid_path(path))
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
