#include <errno.h>
#include <stddef.h>
#include "libft.h"
#include "mocks.h"

static void apply_stat_defaults(struct stat *stats)
{
    // Set stat default values if they are equal to 0 
    (void) stats;
}

static int copy_entry_stats(const t_vfs_mock_entry *entry, struct stat *statbuf)
{
    *statbuf = entry->stats;
    if (entry->author != NULL && vfs_mock_resolve_author(entry->author, &statbuf->st_uid) != 0)
    {
        errno = EINVAL;
        return -1;
    }
    if (entry->group != NULL && vfs_mock_resolve_group(entry->group, &statbuf->st_gid) != 0)
    {
        errno = EINVAL;
        return -1;
    }
    apply_stat_defaults(statbuf);
    return 0;
}

int stat_mock(const char *restrict pathname, struct stat *restrict statbuf)
{
    const t_vfs_mock_entry *entry = find_vfs_entry(pathname);

    if (entry == NULL)
    {
        errno = ENOENT;
        return -1;
    }

    if (entry->errors.stat_errno != 0)
    {
        errno = entry->errors.stat_errno;
        return -1;
    }

    while (S_ISLNK(entry->stats.st_mode))
    {
        entry = find_vfs_entry(entry->target);
        if (entry == NULL)
        {
            errno = ENOENT;
            return -1;
        }
    }

    return copy_entry_stats(entry, statbuf);
}

int lstat_mock(const char *restrict pathname, struct stat *restrict statbuf)
{
    const t_vfs_mock_entry *entry = find_vfs_entry(pathname);

    if (entry == NULL)
    {
        errno = ENOENT;
        return (-1);
    }

    if (entry->errors.lstat_errno != 0)
    {
        errno = entry->errors.lstat_errno;
        return (-1);
    }

    return copy_entry_stats(entry, statbuf);
}
