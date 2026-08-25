#pragma once

void ensure_opendir_will_open_N_dirs_named(const char **dirs_names);
void guarantee_readdir_will_return_N_files_named(const char ***files_names);

void guarantee_opendir_will_fail();
void guarantee_readdir_will_fail();
void reset_dirent_guarantees();

void guarantee_stat_will_populate_stats_of_N_file_types_for_paths(const char **paths, const unsigned int *types);
void guarantee_stat_will_populate_stats_of_a_regular_type_file(const char *file_path);
void guarantee_stat_will_populate_stats_of_a_directory_type_file(const char *dir_path);
void reset_stat_guarantees();

int verify_that_the_str_that_has_been_printed_is(const char *str, ...);
void reset_printing_buffer();
