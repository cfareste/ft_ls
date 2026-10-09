#pragma once

#include <sys/stat.h>

typedef struct s_vfs_mock_errors
{
    unsigned int entry_idx;
    int stat_errno;
    int lstat_errno;
    int opendir_errno;
    int readdir_errno;
    int closedir_errno;
    int readlink_errno;
} t_vfs_mock_errors;

typedef struct s_vfs_mock_entry
{
    const char *path;
    struct stat stats;
    const char * const *entries;
    const char *target;
    t_vfs_mock_errors errors;
} t_vfs_mock_entry;

#define NO_ERRORS ((t_vfs_mock_errors){ 0 })
#define MOCK_ENTRY(path_value, mode_value, entries_value, target_value, errors_value) \
    { \
        .path = (path_value), \
        .stats = { .st_mode = (mode_value), .st_nlink = 1 }, \
        .entries = (entries_value), \
        .target = (target_value), \
        .errors = errors_value \
    }

#define MOCK_FILE(p)                    MOCK_ENTRY((p), S_IFREG | 0644, NULL, NULL, NO_ERRORS)
#define MOCK_DIR(p, ...)                MOCK_ENTRY((p), S_IFDIR | 0755, ((const char *[]){ __VA_ARGS__, NULL }), NULL, NO_ERRORS)
#define MOCK_SYMLINK(p, link_target)    MOCK_ENTRY((p), S_IFLNK | 0777, NULL, (link_target), NO_ERRORS)
#define MOCK_BLOCK_DEVICE(p)            MOCK_ENTRY((p), S_IFBLK | 0660, NULL, NULL, NO_ERRORS)
#define MOCK_CHAR_DEVICE(p)             MOCK_ENTRY((p), S_IFCHR | 0660, NULL, NULL, NO_ERRORS)
#define MOCK_SOCKET(p)                  MOCK_ENTRY((p), S_IFSOCK | 0777, NULL, NULL, NO_ERRORS)
#define MOCK_FIFO(p)                    MOCK_ENTRY((p), S_IFIFO | 0664, NULL, NULL, NO_ERRORS)
#define MOCK_NULL_TERMINATOR()          MOCK_ENTRY(NULL, 0, NULL, NULL, NO_ERRORS)

#define MOCK_FILE_ACCESS_ERROR(err, p)                  MOCK_ENTRY((p), S_IFREG | 0644, NULL, NULL, ((t_vfs_mock_errors){ .stat_errno = (err), .lstat_errno = (err) }))
#define MOCK_DIR_ACCESS_ERROR(err, p, ...)              MOCK_ENTRY((p), S_IFDIR | 0755, ((const char *[]){ __VA_ARGS__, NULL }), NULL, ((t_vfs_mock_errors){ .stat_errno = (err), .lstat_errno = (err) }))
#define MOCK_BROKEN_LINK(p)                             MOCK_ENTRY((p), S_IFLNK | 0755, NULL, NULL, ((t_vfs_mock_errors){ .stat_errno = ENOENT }))
#define MOCK_LOOP_LINK(p)                               MOCK_ENTRY((p), S_IFLNK | 0755, NULL, NULL, ((t_vfs_mock_errors){ .stat_errno = ELOOP }))
#define MOCK_DIR_OPEN_ERROR(err, p, ...)                MOCK_ENTRY((p), S_IFDIR | 0755, ((const char *[]){ __VA_ARGS__, NULL }), NULL, ((t_vfs_mock_errors){ .opendir_errno = (err) }))
#define MOCK_DIR_READ_ERROR(p, entry_error_idx, ...)    MOCK_ENTRY((p), S_IFDIR | 0755, ((const char *[]){ __VA_ARGS__, NULL }), NULL, ((t_vfs_mock_errors){ .entry_idx = (entry_error_idx), .readdir_errno = EBADF }))
#define MOCK_DIR_CLOSE_ERROR(p, ...)                    MOCK_ENTRY((p), S_IFDIR | 0755, ((const char *[]){ __VA_ARGS__, NULL }), NULL, ((t_vfs_mock_errors){ .closedir_errno = EBADF }))
#define MOCK_READ_SYM_LINK_ERROR(err, p, link_target)   MOCK_ENTRY((p), S_IFLNK | 0777, NULL, (link_target), ((t_vfs_mock_errors){ .readlink_errno = (err) }))

void vfs_mock_setup(const t_vfs_mock_entry *entries);
const t_vfs_mock_entry *find_vfs_entry(const char *path);
void vfs_mock_reset(void);

int verify_that_the_output_printed_is(const char *str, ...);
int verify_that_the_error_printed_is(const char *str, ...);
int verify_that_no_output_was_printed(void);
int verify_that_no_error_was_printed(void);
void reset_printing_buffer(void);
