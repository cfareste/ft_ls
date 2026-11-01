#include <errno.h>
#include <stddef.h>
#include <time.h>
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

static const size_t identity_size = sizeof(mock_authors) / sizeof(mock_authors[0]);

time_t vfs_mock_make_timestamp(int day, int month, int year, int hour, int minute, int second)
{
    struct tm date = {
        .tm_sec = second,
        .tm_min = minute,
        .tm_hour = hour,
        .tm_mday = day,
        .tm_mon = month - 1,
        .tm_year = year - 1900,
        .tm_isdst = -1
    };

    return mktime(&date);
}

static const t_mock_identity *find_identity(unsigned int id, const t_mock_identity *identities)
{
    for (size_t i = 0; i < identity_size; i++)
    {
        if (identities[i].id == id)
            return &identities[i];
    }
    return NULL;
}

static const t_mock_identity *find_identity_by_name(const char *name,
    const t_mock_identity *identities)
{
    if (name == NULL)
        return NULL;
    for (size_t i = 0; i < identity_size; i++)
    {
        if (ft_strcmp(name, identities[i].name) == EQUAL_STRINGS)
            return &identities[i];
    }
    return NULL;
}

int vfs_mock_resolve_author(const char *author, uid_t *uid)
{
    const t_mock_identity *identity = find_identity_by_name(author, mock_authors);

    if (uid == NULL || identity == NULL)
        return -1;
    *uid = (uid_t)identity->id;
    return 0;
}

int vfs_mock_resolve_group(const char *group, gid_t *gid)
{
    const t_mock_identity *identity = find_identity_by_name(group, mock_groups);

    if (gid == NULL || identity == NULL)
        return -1;
    *gid = (gid_t)identity->id;
    return 0;
}

const char *vfs_mock_author_name(uid_t uid)
{
    const t_mock_identity *identity = find_identity((unsigned int)uid, mock_authors);

    return identity == NULL ? NULL : identity->name;
}

const char *vfs_mock_group_name(gid_t gid)
{
    const t_mock_identity *identity = find_identity((unsigned int)gid, mock_groups);

    return identity == NULL ? NULL : identity->name;
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
