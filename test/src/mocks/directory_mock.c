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

static short will_opendir_fail = 0;
static short will_readdir_fail = 0;
static char *next_dir_names[256] = { 0 };
static char *expected_opendir_path = NULL;

static void free_dirent_names(void)
{
    for (unsigned int i = 0; next_dir_names[i] != NULL; i++)
    {
        free(next_dir_names[i]);
        next_dir_names[i] = NULL;
    }
}

static void free_expected_opendir_path(void)
{
    if (expected_opendir_path == NULL)
        return ;

    free(expected_opendir_path);
    expected_opendir_path = NULL;
}

DIR *mock_opendir(const char *path)
{
    if (will_opendir_fail == 1 || expected_opendir_path == NULL || ft_strcmp(path, expected_opendir_path) != EQUAL_STRINGS)
        return NULL;

    (void) path;
    t_mock_dir *dir = ft_safe_calloc(1, sizeof(t_mock_dir));
    dir->next_index = 0;

    return (DIR *) dir;
}

struct dirent *mock_readdir(DIR *dirp)
{
    t_mock_dir *dir = (t_mock_dir *) dirp;

    if (will_readdir_fail == 1 || next_dir_names[dir->next_index] == NULL)
        return NULL;

    ft_bzero(&dir->entry, sizeof(dir->entry));
    ft_strlcpy(dir->entry.d_name, next_dir_names[dir->next_index], sizeof(dir->entry.d_name));
    dir->next_index++;

    return &dir->entry;
}

int mock_closedir(DIR *dirp)
{
    free(dirp);
    return 0;
}

void ensure_opendir_will_open_a_dir_named(const char *dir_name)
{
    will_opendir_fail = 0;
    will_readdir_fail = 0;
    free_expected_opendir_path();

    expected_opendir_path = ft_safe_strdup(dir_name);
}

void guarantee_readdir_will_return_N_files_named(const char **files_names)
{
    will_opendir_fail = 0;
    will_readdir_fail = 0;
    free_dirent_names();

    unsigned int i = 0;
    for (; files_names[i] != NULL; i++)
        next_dir_names[i] = ft_safe_strdup(files_names[i]);
    next_dir_names[i] = NULL;
}

void guarantee_readdir_will_return_a_file_named(const char *file_name)
{
    const char *file_names[2] = { file_name, NULL };

    guarantee_readdir_will_return_N_files_named(file_names);
}

void guarantee_opendir_will_fail()
{
    reset_dirent_guarantees();
    will_opendir_fail = 1;
}

void guarantee_readdir_will_fail()
{
    reset_dirent_guarantees();
    will_readdir_fail = 1;
}

void reset_dirent_guarantees()
{
    will_opendir_fail = 0;
    will_readdir_fail = 0;
    free_dirent_names();
    free_expected_opendir_path();
}
