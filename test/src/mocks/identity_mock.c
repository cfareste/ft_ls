#include <pwd.h>
#include <grp.h>
#include "mocks.h"

struct passwd *getpwuid_mock(uid_t uid)
{
    static struct passwd password;
    const char *author = vfs_mock_author_name(uid);

    if (author == NULL)
        return NULL;
    password.pw_name = (char *)author;
    password.pw_passwd = "x";
    password.pw_uid = uid;
    password.pw_gid = DEFAULT_GID;
    password.pw_gecos = (char *)author;
    password.pw_dir = "/home/mock";
    password.pw_shell = "/bin/sh";

    return &password;
}

struct group *getgrgid_mock(gid_t gid)
{
    static struct group group;
    static char *members[] = { NULL };
    const char *group_name = vfs_mock_group_name(gid);

    if (group_name == NULL)
        return NULL;
    group.gr_name = (char *)group_name;
    group.gr_passwd = "x";
    group.gr_gid = gid;
    group.gr_mem = members;

    return &group;
}
