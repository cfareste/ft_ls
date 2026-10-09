#include <errno.h>
#include <stddef.h>
#include "libft.h"
#include "mocks.h"

static const t_vfs_mock_entry *vfs_mock_entries = NULL;

typedef struct s_mock_identity
{
    const char *name;
    unsigned int id;
} t_mock_identity;

static const t_mock_identity mock_authors[] = {
    { "root", 0 },
    { "daemon", 1 },
    { "alice", 1000 },
    { "bob", 1001 },
    { "charlie", 1002 },
    { "nobody", 65534 }
};

static const t_mock_identity mock_groups[] = {
    { "root", 0 },
    { "wheel", 10 },
    { "staff", 50 },
    { "users", 100 },
    { "developers", 1000 },
    { "nogroup", 65534 }
};

static int resolve_identity(const char *name, const t_mock_identity *identities,
    size_t identity_count, unsigned int *id)
{
    if (name == NULL || id == NULL)
        return -1;

    for (size_t i = 0; i < identity_count; i++)
    {
        if (ft_strcmp(name, identities[i].name) == EQUAL_STRINGS)
        {
            *id = identities[i].id;
            return 0;
        }
    }
    return -1;
}

int vfs_mock_resolve_author(const char *author, uid_t *uid)
{
    unsigned int id;

    if (uid == NULL || resolve_identity(author, mock_authors,
            sizeof(mock_authors) / sizeof(mock_authors[0]), &id) != 0)
        return -1;
    *uid = (uid_t)id;
    return 0;
}

int vfs_mock_resolve_group(const char *group, gid_t *gid)
{
    unsigned int id;

    if (gid == NULL || resolve_identity(group, mock_groups,
            sizeof(mock_groups) / sizeof(mock_groups[0]), &id) != 0)
        return -1;
    *gid = (gid_t)id;
    return 0;
}

void vfs_mock_setup(const t_vfs_mock_entry *entries)
{
    vfs_mock_entries = entries;
}

const t_vfs_mock_entry *find_vfs_entry(const char *path)
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
    errno = 0;
    vfs_mock_entries = NULL;
}
