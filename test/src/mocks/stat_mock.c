#include <stddef.h>
#include "libft.h"
#include "mocks.h"

int stat_mock(const char *restrict pathname, struct stat *restrict statbuf)
{
    const t_vfs_mock_entry *entry = find_vfs_entry(pathname);
    if (entry == NULL)
        return -1;

    while (S_ISLNK(entry->mode))
    {
        entry = find_vfs_entry(entry->target);
        if (entry == NULL)
            return -1;
    }

    ft_bzero(statbuf, sizeof(struct stat));
    statbuf->st_mode = entry->mode;
    return 0;
}
