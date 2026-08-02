#include <assert.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include "libft.h"
#include "mocks.h"

typedef struct s_mock_dir
{
    struct dirent entry;
    unsigned int next_index;
} t_mock_dir;

static short g_opendir_fail = 0;
static short g_readdir_fail = 0;
static char *g_dirent_names[256] = {0};

static void free_dirent_names(void)
{
    for (unsigned int i = 0; g_dirent_names[i] != NULL; i++)
    {
        free(g_dirent_names[i]);
        g_dirent_names[i] = NULL;
    }
}

DIR *mock_opendir(const char *path)
{
    if (g_opendir_fail == 1 || path[0] == '\0')
        return NULL;

    t_mock_dir *dir = ft_safe_calloc(1, sizeof(t_mock_dir));
    dir->next_index = 0;

    return (DIR *) dir;
}

struct dirent *mock_readdir(DIR *dirp)
{
    t_mock_dir *dir = (t_mock_dir *) dirp;

    if (g_readdir_fail == 1 || dir == NULL || g_dirent_names[dir->next_index] == NULL)
        return NULL;

    ft_bzero(&dir->entry, sizeof(dir->entry));
    ft_strlcpy(dir->entry.d_name, g_dirent_names[dir->next_index], sizeof(dir->entry.d_name));
    dir->next_index++;

    return &dir->entry;
}

int mock_closedir(DIR *dirp)
{
    free(dirp);
    return 0;
}


void guarantee_readdir_will_return_N_files_named(const char **files_names)
{
    g_opendir_fail = 0;
    g_readdir_fail = 0;

    free_dirent_names();

    unsigned int i = 0;
    for (; files_names[i] != NULL; i++)
        g_dirent_names[i] = ft_strdup(files_names[i]);
    g_dirent_names[i] = NULL;
}

void guarantee_readdir_will_return_a_file_named(const char *file_name)
{
    const char *file_names[2] = {file_name, NULL};

    guarantee_readdir_will_return_N_files_named(file_names);
}

void guarantee_opendir_will_fail()
{
    g_readdir_fail = 0;
    g_opendir_fail = 1;
    free_dirent_names();
}

void guarantee_readdir_will_fail()
{
    g_opendir_fail = 0;
    g_readdir_fail = 1;
    free_dirent_names();
}

void reset_dirent_guarantees()
{
    g_opendir_fail = 0;
    g_readdir_fail = 0;
    free_dirent_names();
}
