#include <dirent.h>
#include <stdlib.h>
#include "libft.h"
#include "mocks.h"

typedef struct s_mock_dir
{
    const t_vfs_mock_entry *dir_entry;
    struct dirent entry;
    unsigned int next_index;
} t_mock_dir;

DIR *mock_opendir(const char *path)
{
    const t_vfs_mock_entry *dir_entry = find_vfs_entry(path);

    if (dir_entry != NULL && S_ISDIR(dir_entry->mode))
    {
        t_mock_dir *dir = ft_safe_calloc(1, sizeof(t_mock_dir));
        dir->dir_entry = dir_entry;
        dir->next_index = 0;
        return (DIR *) dir;
    }

    return NULL;
}

struct dirent *mock_readdir(DIR *dirp)
{
    t_mock_dir *stream = (t_mock_dir *)dirp;

    if (stream != NULL && stream->dir_entry != NULL && stream->dir_entry->entries != NULL)
    {
        const char *next_entry_name = stream->dir_entry->entries[stream->next_index];
        if (next_entry_name == NULL)
            return NULL;

        ft_bzero(&stream->entry, sizeof(struct dirent));
        ft_strlcpy(stream->entry.d_name, next_entry_name, sizeof(stream->entry.d_name));
        stream->next_index++;

        return &stream->entry;
    }

    return NULL;
}

int mock_closedir(DIR *dirp)
{
    t_mock_dir *dir = (t_mock_dir *) dirp;

    free(dir);

    return 0;
}