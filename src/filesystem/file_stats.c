#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "libft.h"
#include "file_stats.h"
#include "error_reporter.h"

#define STATS_RETRIEVAL_ERROR 0
#define STATS_RETRIEVAL_SUCCESS 1

struct s_file_stats
{
    t_file_type type;
};

static int should_retrieve_with_lstat(const int retrieve_error, const struct stat_mock *stats)
{
    return retrieve_error == -1
           ? (errno == ENOENT || errno == ELOOP)
           : !S_ISDIR(stats->st_mode);
}

static int retrieve_file_stats(const char *file_path, struct stat *stats)
{
    int retrieve_error = stat(file_path, stats);

    if (should_retrieve_with_lstat(retrieve_error, stats))
    {
        retrieve_error = lstat(file_path, stats);
    }

    if (retrieve_error == -1)
    {
        report_access_file_error(file_path);
        return STATS_RETRIEVAL_ERROR;
    }

    return STATS_RETRIEVAL_SUCCESS;
}

static t_file_type get_file_type(const mode_t mode)
{
    t_file_type file_type = FILE_TYPE_UNKNOWN;

    if (S_ISREG(mode))
        file_type = FILE_TYPE_REGULAR;
    if (S_ISDIR(mode))
        file_type = FILE_TYPE_DIRECTORY;
    if (S_ISCHR(mode))
        file_type = FILE_TYPE_CHARDEVICE;
    if (S_ISBLK(mode))
        file_type = FILE_TYPE_BLOCKDEVICE;
    if (S_ISFIFO(mode))
        file_type = FILE_TYPE_FIFO;
    if (S_ISLNK(mode))
        file_type = FILE_TYPE_SYMLINK;
    if (S_ISSOCK(mode))
        file_type = FILE_TYPE_SOCKET;

    return file_type;
}

t_file_stats *file_stats_get(const char *file_path)
{
    if (!ft_is_valid_path(file_path))
        return NULL;

    struct stat stats;
    if (retrieve_file_stats(file_path, &stats) == STATS_RETRIEVAL_ERROR)
        return NULL;

    t_file_stats *file_stats = ft_safe_calloc(1, sizeof(t_file_stats));
    file_stats->type = get_file_type(stats.st_mode);

    return file_stats;
}

t_file_type file_stats_get_file_type(const t_file_stats *file_stats)
{
    if (file_stats == NULL)
        return FILE_TYPE_UNKNOWN;

    return file_stats->type;
}

void file_stats_destroy(t_file_stats **file_stats)
{
    if (file_stats == NULL || *file_stats == NULL)
        return ;

    free(*file_stats);
    *file_stats = NULL;
}
