#include <errno.h>
#include <string.h>
#include <sys/xattr.h>
#include <sys/types.h>
#include "mocks.h"

ssize_t lgetxattr_mock(const char *path, const char *name, void *value, size_t size)
{
    if (path == NULL || name == NULL)
    {
        errno = EFAULT;
        return -1;
    }
    if (path[0] == '\0')
    {
        errno = ENOENT;
        return -1;
    }
    if (name[0] == '\0')
    {
        errno = EINVAL;
        return -1;
    }

    const t_vfs_mock_entry *entry = find_vfs_entry(path);
    if (entry == NULL)
    {
        errno = ENOENT;
        return -1;
    }
    if (entry->errors.lgetxattr_errno != 0)
    {
        errno = entry->errors.lgetxattr_errno;
        return -1;
    }

    if (entry->xattrs != NULL)
    {
        for (size_t i = 0; entry->xattrs[i].name != NULL; i++)
        {
            const t_vfs_mock_xattr *xattr = &entry->xattrs[i];

            if (strcmp(xattr->name, name) != 0)
                continue;
            if (value == NULL)
                return (ssize_t)xattr->size;
            if (size < xattr->size)
            {
                errno = ERANGE;
                return -1;
            }
            if (xattr->size > 0 && xattr->value == NULL)
            {
                errno = EINVAL;
                return -1;
            }
            memcpy(value, xattr->value, xattr->size);
            return (ssize_t)xattr->size;
        }
    }

    errno = ENODATA;
    return -1;
}
