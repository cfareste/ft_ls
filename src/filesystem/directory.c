#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include "libft.h"
#include "directory.h"
#include "error_reporter.h"

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
        report_opening_directory_error(path);
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
        report_reading_directory_error(dir_stream->dir_name);
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
        report_closing_directory_error((* dir_stream)->dir_name);

    free((*dir_stream)->dir_name);
    free(*dir_stream);
    *dir_stream = NULL;

    return result;
}
