#include <dirent.h>
#include <stdlib.h>
#include "libft.h"
#include "directory.h"

struct s_dir_stream
{
    DIR *dir;
};

struct s_dir_entry
{
    struct dirent *entry;
};

t_dir_stream *directory_open(const char *path)
{
    if (!ft_is_valid_path(path))
        return NULL;

    t_dir_stream *dir_stream = ft_safe_calloc(1, sizeof(t_dir_stream));
    dir_stream->dir = opendir(path);

    return dir_stream;
}

struct dirent *directory_get_next_entry(t_dir_stream *stream)
{
    if (stream == NULL)
        return NULL;

    return readdir(stream->dir);
}

int directory_close(t_dir_stream **dir_stream)
{
    if (dir_stream == NULL || *dir_stream == NULL)
        return -1;

    const int result = closedir((*dir_stream)->dir);
    free(*dir_stream);
    *dir_stream = NULL;

    return result;
}
