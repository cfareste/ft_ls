#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "libft.h"
#include "directory.h"

struct s_dir_stream
{
    DIR *dir;
    char *dir_name;
};

struct s_dir_entry
{
    struct dirent *entry;
};

t_dir_stream *directory_open(const char *path)
{
    if (!ft_is_valid_path(path))
        return NULL;

    DIR *dir = opendir(path);

    if (dir == NULL)
    {
        ft_fprintf(STDERR_FILENO, "ft_ls: cannot open directory '%s': %s\n", path, strerror(errno));
        return NULL;
    }

    t_dir_stream *dir_stream = ft_safe_calloc(1, sizeof(t_dir_stream));
    dir_stream->dir = dir;
    dir_stream->dir_name = ft_safe_strdup(path);

    return dir_stream;
}

t_dir_entry *directory_get_next_entry(const t_dir_stream *dir_stream)
{
    if (dir_stream == NULL)
        return NULL;

    errno = 0;
    struct dirent *entry = readdir(dir_stream->dir);

    if (entry == NULL && errno != 0)
    {
        ft_fprintf(STDERR_FILENO, "ft_ls: reading directory '%s': %s\n", dir_stream->dir_name, strerror(errno));
        return NULL;
    }

    t_dir_entry *dir_entry = ft_safe_calloc(1, sizeof(t_dir_entry));
    dir_entry->entry = entry;
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

int directory_is_entry_hidden_file(const t_dir_entry *dir_entry)
{
    if (dir_entry == NULL)
        return 0;

    return dir_entry->entry->d_name[0] == '.';
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

    if (result != 0)
    {
        ft_fprintf(STDERR_FILENO, "ft_ls: closing directory '%s': %s\n", (*dir_stream)->dir_name, strerror(errno));
    }

    free((*dir_stream)->dir_name);
    free(*dir_stream);
    *dir_stream = NULL;

    return result;
}
