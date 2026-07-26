#include <dirent.h>

#include "libft.h"
#include "os_adapter.h"

struct dir_stream
{
    DIR *dir;
};

struct dir_entry
{
    struct dirent *entry;
};

t_dir_stream *opendir_adapter(const char *path)
{
    t_dir_stream *dir_stream = ft_calloc(1, sizeof(t_dir_stream));

    dir_stream->dir = opendir(path);

    return dir_stream;
}

t_dir_entry *readdir_adapter(t_dir_stream *dir)
{
    t_dir_entry *dir_entry = ft_calloc(1, sizeof(t_dir_entry));

    dir_entry->entry = readdir(dir->dir);

    return dir_entry;
}

int closedir_adapter(t_dir_stream **dir_stream)
{
    if (dir_stream == NULL || *dir_stream == NULL)
        return 0;

    const int result = closedir((*dir_stream)->dir);
    free(*dir_stream);
    *dir_stream = NULL;

    return result;
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

short dir_entry_is_null(const t_dir_entry *dir_entry)
{
    return (short) (dir_entry->entry == NULL);
}
