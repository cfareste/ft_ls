#pragma once

#include <sys/stat.h>

#define MOCK_FILE(p) { (p), S_IFREG | 0644, NULL, NULL }
#define MOCK_DIR(p, ...) { (p), S_IFDIR | 0755, (const char *[]){ __VA_ARGS__, NULL }, NULL }
#define MOCK_SYMLINK(p, target) { (p), S_IFLNK | 0777, NULL, (target) }

typedef struct s_vfs_mock_entry
{
    const char *path;
    mode_t mode;
    const char * const *entries;
} t_vfs_mock_entry;

void vfs_mock_setup(const t_vfs_mock_entry *entries);
const t_vfs_mock_entry *find_vfs_entry(const char *path);
void vfs_mock_reset(void);

void ensure_opendir_will_open_N_dirs_named(const char **dirs_names);
void guarantee_readdir_will_return_N_files_named(const char ***files_names);

void guarantee_opendir_will_fail(void);
void guarantee_readdir_will_fail(void);
void reset_dirent_guarantees(void);

void guarantee_stat_will_populate_stats_of_N_file_types_for_paths(const char **paths, const unsigned int *types);
void guarantee_stat_will_populate_stats_of_a_regular_type_file(const char *file_path);
void guarantee_stat_will_populate_stats_of_a_directory_type_file(const char *dir_path);
void reset_stat_guarantees(void);

int verify_that_the_str_that_has_been_printed_is(const char *str, ...);
void reset_printing_buffer(void);
