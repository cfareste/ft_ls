#include <dirent.h>
#include <stdlib.h>
#include "libft.h"
#include "directory.h"

struct dir_stream
{
    DIR *dir;
};

struct dir_entry
{
    struct dirent *entry;
};


static int is_invalid_path(const char *path)
{
    return path == NULL || path[0] == '\0';
}

t_dir_stream *dir_stream_open(const char *path)
{
    if (is_invalid_path(path))
        return NULL;

    t_dir_stream *dir_stream = ft_safe_calloc(1, sizeof(t_dir_stream));
    dir_stream->dir = opendir(path);
    if (dir_stream->dir == NULL)
    {
        free(dir_stream);
        return NULL;
    }

    return dir_stream;
}

int dir_stream_close(t_dir_stream **dir_stream)
{
    if (dir_stream == NULL || *dir_stream == NULL)
        return 0;

    const int result = closedir((*dir_stream)->dir);
    free(*dir_stream);
    *dir_stream = NULL;

    return result;
}

t_dir_entry *dir_stream_get_next_entry(t_dir_stream *dir)
{
    t_dir_entry *dir_entry = ft_safe_calloc(1, sizeof(t_dir_entry));

    dir_entry->entry = readdir(dir->dir);

    return dir_entry;
}

void dir_entry_destroy(t_dir_entry **dir_entry)
{
    if (!dir_entry || !*dir_entry)
        return;

    free(*dir_entry);
}

const char *dir_entry_get_name(const t_dir_entry *dir_entry)
{
    return dir_entry->entry->d_name;
}

short dir_entry_is_empty(const t_dir_entry *dir_entry)
{
    return (short) (dir_entry->entry == NULL);
}
