#include <stdlib.h>
#include <sys/stat.h>
#include "libft.h"

static char *next_paths[256] = { NULL };
static unsigned int next_types[256] = { 0 };

static void free_stat_guarantees(void)
{
    for (unsigned int i = 0; next_paths[i] != NULL; i++)
    {
        free(next_paths[i]);
        next_paths[i] = NULL;
    }
    next_types[0] = 0;
}

int stat_mock(const char *restrict pathname, struct stat *restrict statbuf)
{
    for (unsigned int i = 0; next_paths[i] != NULL; i++)
    {
        if (ft_strcmp(pathname, next_paths[i]) == EQUAL_STRINGS)
        {
            statbuf->st_mode = next_types[i];
            return 0;
        }
    }

    return -1;
}

void guarantee_stat_will_populate_stats_of_N_file_types_for_paths(const char **paths, const unsigned int *types)
{
    free_stat_guarantees();

    unsigned int i = 0;
    for (; paths[i] != NULL; i++)
    {
        next_paths[i] = ft_safe_strdup(paths[i]);
        next_types[i] = types[i];
    }
    next_paths[i] = NULL;
    next_types[i] = 0;
}

void guarantee_stat_will_populate_stats_of_a_regular_type_file(const char *file_path)
{
    const char *path[] = { file_path, NULL };
    const unsigned int type[] = { S_IFREG, 0 };

    guarantee_stat_will_populate_stats_of_N_file_types_for_paths(path, type);
}

void guarantee_stat_will_populate_stats_of_a_directory_type_file(const char *dir_path)
{
    const char *path[] = { dir_path, NULL };
    const unsigned int type[] = { S_IFDIR, 0 };

    guarantee_stat_will_populate_stats_of_N_file_types_for_paths(path, type);
}

void reset_stat_guarantees(void)
{
    free_stat_guarantees();
}
