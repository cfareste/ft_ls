#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include "libft.h"
#include "mocks.h"

typedef struct s_mock_dir
{
    char *dir_name;
    struct dirent entry;
    unsigned int next_index;
} t_mock_dir;

static short will_opendir_fail = 0;
static short will_readdir_fail = 0;
static char *next_dir_names[16][256] = { 0 };
static char *expected_opendir_paths[16] = { NULL };

static void free_dirent_names(void)
{
    for (unsigned int i = 0; i < 16; i++)
    {
        for (unsigned int j = 0; next_dir_names[i][j] != NULL; j++)
        {
            free(next_dir_names[i][j]);
            next_dir_names[i][j] = NULL;
        }
    }
}

static void free_expected_opendir_path(void)
{
    for (unsigned int i = 0; expected_opendir_paths[i] != NULL; i++)
    {
        free(expected_opendir_paths[i]);
        expected_opendir_paths[i] = NULL;
    }
}

static int get_dir_index(const char *dir_name)
{
    for (int i = 0; expected_opendir_paths[i] != NULL; i++)
    {
        if (ft_strcmp(expected_opendir_paths[i], dir_name) == EQUAL_STRINGS)
        {
            return i;
        }
    }

    return -1;
}

static char *get_next_file_entry_name(const t_mock_dir *dir)
{
    const int dir_index = get_dir_index(dir->dir_name);

    if (dir_index == -1)
    {
        return NULL;
    }

    return next_dir_names[dir_index][dir->next_index];
}

DIR *mock_opendir(const char *path)
{
    if (will_opendir_fail == 1 || get_dir_index(path) == -1)
        return NULL;

    t_mock_dir *dir = ft_safe_calloc(1, sizeof(t_mock_dir));
    dir->next_index = 0;
    dir->dir_name = ft_safe_strdup(path);

    return (DIR *) dir;
}

struct dirent *mock_readdir(DIR *dirp)
{
    t_mock_dir *dir = (t_mock_dir *) dirp;

    const char *expected_file_entry_name = get_next_file_entry_name(dir);

    if (will_readdir_fail == 1 || expected_file_entry_name == NULL)
        return NULL;

    ft_bzero(&dir->entry, sizeof(dir->entry));
    ft_strlcpy(dir->entry.d_name, expected_file_entry_name, sizeof(dir->entry.d_name));
    dir->next_index++;

    return &dir->entry;
}

int mock_closedir(DIR *dirp)
{
    t_mock_dir *dir = (t_mock_dir *) dirp;

    free(dir->dir_name);
    free(dir);

    return 0;
}

void ensure_opendir_will_open_N_dirs_named(const char **dirs_names)
{
    will_opendir_fail = 0;
    will_readdir_fail = 0;
    free_expected_opendir_path();

    unsigned int i = 0;
    for (; dirs_names[i] != NULL; i++)
        expected_opendir_paths[i] = ft_safe_strdup(dirs_names[i]);
    expected_opendir_paths[i] = NULL;
}

void guarantee_readdir_will_return_N_files_named(const char ***files_names)
{
    will_opendir_fail = 0;
    will_readdir_fail = 0;
    free_dirent_names();

    for (unsigned int i = 0; i < 16; i++)
    {
        if (files_names[i] == NULL)
            break ;

        unsigned int j = 0;
        for (; files_names[i][j] != NULL; j++)
        {
            next_dir_names[i][j] = ft_safe_strdup(files_names[i][j]);
        }
        next_dir_names[i][j] = NULL;
    }
}

void guarantee_opendir_will_fail(void)
{
    reset_dirent_guarantees();
    will_opendir_fail = 1;
}

void guarantee_readdir_will_fail(void)
{
    reset_dirent_guarantees();
    will_readdir_fail = 1;
}

void reset_dirent_guarantees(void)
{
    will_opendir_fail = 0;
    will_readdir_fail = 0;
    free_dirent_names();
    free_expected_opendir_path();
}
