#include <errno.h>
#include <stddef.h>
#include <sys/types.h>
#include "mocks.h"
#include "libft.h"

ssize_t readlink_mock(const char *symlink_path, char *target_buff, size_t buff_len)
{
    if (symlink_path == NULL || target_buff == NULL)
    {
        errno = EFAULT;
        return (-1);
    }

    if (symlink_path[0] == '\0')
    {
        errno = ENOENT;
        return (-1);
    }

    const t_vfs_mock_entry *entry = find_vfs_entry(symlink_path);
    if (entry == NULL)
    {
        errno = ENOENT;
        return (-1);
    }

    if (entry->errors.readlink_errno != 0)
    {
        errno = entry->errors.readlink_errno;
        return (-1);
    }

    if (!S_ISLNK(entry->stats.st_mode) || entry->target == NULL)
    {
        errno = EINVAL;
        return (-1);
    }

    const size_t target_len = ft_strlen(entry->target);
    const size_t bytes_to_copy = (target_len < buff_len) ? target_len : buff_len;

    ft_memcpy(target_buff, entry->target, bytes_to_copy);

    return ((ssize_t)bytes_to_copy);
}
