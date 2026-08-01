#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include "libft.h"
#include "mocks.h"

typedef struct s_mock_dir
{
    struct dirent entry;
    unsigned int next_index;
    unsigned int total_entries;
} t_mock_dir;

static short g_opendir_fail = 0;
static short g_readdir_fail = 0;
static char *g_dirent_names[256] = {0};
static unsigned int g_total_dirent_entries = 0;

static void free_dirent_names(void)
{
    for (unsigned int i = 0; i < g_total_dirent_entries; i++)
    {
        free(g_dirent_names[i]);
        g_dirent_names[i] = NULL;
    }

    g_total_dirent_entries = 0;
}

DIR *mock_opendir(const char *path)
{
    (void) path;

    if (g_opendir_fail == 1)
        return NULL;

    t_mock_dir *dir = ft_safe_calloc(1, sizeof(t_mock_dir));
    dir->next_index = 0;
    dir->total_entries = g_total_dirent_entries;

    return (DIR *) dir;
}

struct dirent *mock_readdir(DIR *dirp)
{
    t_mock_dir *dir = (t_mock_dir *) dirp;

    if (g_readdir_fail == 1 || dir == NULL || dir->next_index >= dir->total_entries)
        return NULL;

    memset(&dir->entry, 0, sizeof(dir->entry));
    ft_strlcpy(dir->entry.d_name, g_dirent_names[dir->next_index], sizeof(dir->entry.d_name));
    dir->next_index++;

    return &dir->entry;
}

int mock_closedir(DIR *dirp)
{
    free(dirp);
    return 0;
}


void guarantee_readdir_will_return_N_files_named(const char **files_names, const unsigned int num)
{
    g_opendir_fail = 0;
    g_readdir_fail = 0;
    free_dirent_names();
    g_total_dirent_entries = num;

    for (unsigned int i = 0; i < num; i++)
        g_dirent_names[i] = ft_strdup(files_names[i]);
}

void guarantee_readdir_will_return_a_file_named(const char *file_name)
{
    const char *file_names[1] = {file_name};

    guarantee_readdir_will_return_N_files_named(file_names, 1);
}

void guarantee_opendir_will_fail()
{
    g_readdir_fail = 0;
    g_opendir_fail = 1;
    free_dirent_names();
    g_total_dirent_entries = 0;
}

void guarantee_readdir_will_fail()
{
    g_opendir_fail = 0;
    g_readdir_fail = 1;
    free_dirent_names();
    g_total_dirent_entries = 0;
}

void reset_dirent_guarantees()
{
    g_opendir_fail = 0;
    g_readdir_fail = 0;
    free_dirent_names();
}
