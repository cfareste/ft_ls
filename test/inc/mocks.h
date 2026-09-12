#pragma once

#include <sys/stat.h>

#define MOCK_FILE(p) { (p), S_IFREG | 0644, NULL, NULL, { 0, 0, { 0, 0 }, 0 } }
#define MOCK_DIR(p, ...) { (p), S_IFDIR | 0755, (const char *[]){ __VA_ARGS__, NULL }, NULL, { 0, 0, { 0, 0 }, 0 } }
#define MOCK_SYMLINK(p, target) { (p), S_IFLNK | 0777, NULL, (target), { 0, 0, { 0, 0 }, 0 } }
#define MOCK_BLOCK_DEVICE(p) { (p), S_IFBLK | 0660, NULL, NULL, { 0, 0, { 0, 0 }, 0 } }
#define MOCK_CHAR_DEVICE(p) { (p), S_IFCHR | 0660, NULL, NULL, { 0, 0, { 0, 0 }, 0 } }
#define MOCK_SOCKET(p) { (p), S_IFSOCK | 0777, NULL, NULL, { 0, 0, { 0, 0 }, 0 } }
#define MOCK_FIFO(p) { (p), S_IFIFO | 0664, NULL, NULL, { 0, 0, { 0, 0 }, 0 } }
#define MOCK_NULL_TERMINATOR() { NULL, 0, NULL, NULL, { 0, 0, { 0, 0 }, 0 } }

#define MOCK_DIR_OPEN_ERROR(err, p, ...) { (p), S_IFDIR | 0755, (const char *[]){ __VA_ARGS__, NULL }, NULL, { 0, err, { 0, 0 }, 0 } }
#define MOCK_DIR_READ_ERROR(p, entry_error_idx, ...) { (p), S_IFDIR | 0755, (const char *[]){ __VA_ARGS__, NULL }, NULL, { 0, 0, { entry_error_idx, EBADF } , 0 } }
#define MOCK_DIR_CLOSE_ERROR(p, ...) { (p), S_IFDIR | 0755, (const char *[]){ __VA_ARGS__, NULL }, NULL, { 0, 0, { 0, 0 }, EBADF } }

typedef struct s_vfs_mock_entry
{
    const char *path;
    mode_t mode;
    const char * const *entries;
    const char *target;
    struct
    {
        int stat_errno;
        int opendir_errno;
        struct
        {
            unsigned int entry_idx;
            int code;
        } readdir_error;
        int closedir_errno;
    } errors;
} t_vfs_mock_entry;

void vfs_mock_setup(const t_vfs_mock_entry *entries);
const t_vfs_mock_entry *find_vfs_entry(const char *path);
void vfs_mock_reset(void);

int verify_that_the_output_printed_is(const char *str, ...);
int verify_that_the_error_printed_is(const char *str, ...);
int verify_that_no_output_was_printed(void);
int verify_that_no_error_was_printed(void);
void reset_printing_buffer(void);
