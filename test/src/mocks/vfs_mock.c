#include <stddef.h>
#include "libft.h"
#include "mocks.h"

const t_vfs_mock_entry *g_mock_entries = NULL;

void vfs_mock_setup(const t_vfs_mock_entry *entries)
{
    g_mock_entries = entries;
}

const t_vfs_mock_entry *find_node(const char *path)
{
    if (g_mock_entries == NULL || path == NULL)
        return NULL;

    for (size_t i = 0; g_mock_entries[i].path != NULL; i++)
    {
        if (ft_strcmp(g_mock_entries[i].path, path) == EQUAL_STRINGS)
            return &g_mock_entries[i];
    }

    return NULL;
}

void vfs_mock_reset(void)
{
    g_mock_entries = NULL;
}
