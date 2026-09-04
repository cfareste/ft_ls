#include <stddef.h>
#include "libft.h"
#include "mocks.h"

static const t_vfs_mock_entry *vfs_mock_entries = NULL;

void vfs_mock_setup(const t_vfs_mock_entry *entries)
{
    vfs_mock_entries = entries;
}

const t_vfs_mock_entry *find_node(const char *path)
{
    if (vfs_mock_entries == NULL || path == NULL)
        return NULL;

    for (size_t i = 0; vfs_mock_entries[i].path != NULL; i++)
    {
        if (ft_strcmp(vfs_mock_entries[i].path, path) == EQUAL_STRINGS)
            return &vfs_mock_entries[i];
    }

    return NULL;
}

void vfs_mock_reset(void)
{
    vfs_mock_entries = NULL;
}
