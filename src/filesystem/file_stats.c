#include <grp.h>
#include <pwd.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "libft.h"
#include "file_stats.h"
#include "error_reporter.h"

#define STATS_RETRIEVAL_ERROR (-1)
#define STATS_RETRIEVAL_SUCCESS 0
#define DEFAULT_TARGET_BUFFER_SIZE 128
#define MAX_SAFE_TARGET_BUFFER_SIZE ((SIZE_MAX - 1) / 2)

struct s_file_stats
{
    char *file_name;
    t_file_type type;
    char *target;
    uid_t author_id;
    gid_t group_id;
    nlink_t link_count;
    char *author;
    char *group;
};

static int should_retrieve_file_stats_without_following_symlinks(const int retrieve_error, const struct stat *stats)
{
    return retrieve_error == -1
           ? (errno == ENOENT || errno == ELOOP)
           : !S_ISDIR(stats->st_mode);
}

static int handle_retrieve_error(const int retrieve_error, const char *file_path)
{
    if (retrieve_error != -1)
       return STATS_RETRIEVAL_SUCCESS;

    report_access_file_error(file_path);
    return STATS_RETRIEVAL_ERROR;
}

static int retrieve_file_stats(const char *file_path, struct stat *stats)
{
    int retrieve_error = stat(file_path, stats);

    if (should_retrieve_file_stats_without_following_symlinks(retrieve_error, stats))
        retrieve_error = lstat(file_path, stats);

    return handle_retrieve_error(retrieve_error, file_path);
}

static int retrieve_file_stats_without_following_symlinks(const char *file_path, struct stat *stats)
{
    if (lstat(file_path, stats) == 0)
        return STATS_RETRIEVAL_SUCCESS;

    report_access_file_error(file_path);
    return STATS_RETRIEVAL_ERROR;
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

static void initialize_stats(t_file_stats *file_stats, const char *file_path, const struct stat *stats)
{
    file_stats->type = get_file_type(stats->st_mode);
    file_stats->file_name = ft_safe_strdup(file_path);
    file_stats->author_id = stats->st_uid;
    file_stats->group_id = stats->st_gid;
    file_stats->link_count = stats->st_nlink;
}

static void retrieve_target_pointed_by_link(t_file_stats *stats)
{
    size_t target_buffer_size = DEFAULT_TARGET_BUFFER_SIZE;
    stats->target = ft_safe_calloc(target_buffer_size + 1, sizeof(char));

    while (target_buffer_size <= MAX_SAFE_TARGET_BUFFER_SIZE)
    {
        const ssize_t target_length = readlink(stats->file_name, stats->target, target_buffer_size);

        if (target_length == -1)
        {
            report_read_symbolic_link_error(stats->file_name);
            free(stats->target);
            stats->target = NULL;
            return ;
        }

        if ((size_t)target_length < target_buffer_size)
            return ;

        const size_t new_buffer_size = target_buffer_size * 2;
        stats->target = ft_safe_realloc(stats->target, target_buffer_size + 1, new_buffer_size + 1);
        target_buffer_size = new_buffer_size;
    }

    free(stats->target);
    stats->target = NULL;
}

t_file_stats *file_stats_get(const char *file_path)
{
    if (!ft_is_valid_path(file_path))
        return NULL;

    struct stat stats;
    if (retrieve_file_stats(file_path, &stats) == STATS_RETRIEVAL_ERROR)
        return NULL;

    t_file_stats *file_stats = ft_safe_calloc(1, sizeof(t_file_stats));
    initialize_stats(file_stats, file_path, &stats);
    return file_stats;
}

t_file_stats *file_stats_get_without_following_symlinks(const char *file_path)
{
    if (!ft_is_valid_path(file_path))
        return NULL;

    struct stat stats;
    if (retrieve_file_stats_without_following_symlinks(file_path, &stats) == STATS_RETRIEVAL_ERROR)
        return NULL;

    t_file_stats *file_stats = ft_safe_calloc(1, sizeof(t_file_stats));
    initialize_stats(file_stats, file_path, &stats);
    return file_stats;
}

t_file_type file_stats_get_file_type(const t_file_stats *file_stats)
{
    if (file_stats == NULL)
        return FILE_TYPE_UNKNOWN;

    return file_stats->type;
}

nlink_t file_stats_get_link_count(const t_file_stats *file_stats)
{
    if (file_stats == NULL)
        return 0;

    return file_stats->link_count;
}

const char *file_stats_get_author(t_file_stats *stats)
{
    if (stats == NULL)
        return NULL;

    if (stats->author != NULL)
        return stats->author;

    const struct passwd *author = getpwuid(stats->author_id);

    if (author == NULL || author->pw_name == NULL)
        stats->author = ft_safe_itoa((int)stats->author_id);
    else
        stats->author = ft_safe_strdup(author->pw_name);

    return stats->author;
}

const char *file_stats_get_group(t_file_stats *stats)
{
    if (stats == NULL)
        return NULL;

    if (stats->group != NULL)
        return stats->group;

    const struct group *group = getgrgid(stats->group_id);

    if (group == NULL || group->gr_name == NULL)
        stats->group = ft_safe_itoa((int)stats->group_id);
    else
        stats->group = ft_safe_strdup(group->gr_name);

    return stats->group;
}

const char *file_stats_get_target_pointed_by_link(t_file_stats *stats)
{
    if (stats == NULL || stats->type != FILE_TYPE_SYMLINK)
        return NULL;

    if (stats->target == NULL)
        retrieve_target_pointed_by_link(stats);

    return stats->target;
}

void file_stats_destroy(t_file_stats **file_stats)
{
    if (file_stats == NULL || *file_stats == NULL)
        return ;

    free((*file_stats)->file_name);
    free((*file_stats)->target);
    free((*file_stats)->author);
    free((*file_stats)->group);
    free(*file_stats);
    *file_stats = NULL;
}
