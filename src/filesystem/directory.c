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

//TODO: rename to directory_get_next_entry
t_dir_entry *directory_get_next(const t_dir_stream *dir_stream)
{
    if (dir_stream == NULL)
        return NULL;

    t_dir_entry *dir_entry = ft_safe_calloc(1, sizeof(t_dir_entry));
    dir_entry->entry = readdir(dir_stream->dir);
    return dir_entry;
}

const char *directory_get_entry_name(const t_dir_entry *dir_entry)
{
    if (dir_entry == NULL)
        return NULL;

    return dir_entry->entry->d_name;
}

int directory_is_entry_empty(const t_dir_entry *dir_entry)
{
    if (dir_entry == NULL)
        return 1;

    return dir_entry->entry == NULL;
}

struct dirent *directory_get_next_entry(t_dir_stream *stream)
{
    if (stream == NULL)
        return NULL;

    return readdir(stream->dir);
}

void directory_destroy_entry(t_dir_entry **dir_entry)
{
    if (dir_entry == NULL || *dir_entry == NULL)
        return ;

    free(*dir_entry);
    *dir_entry = NULL;
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
