#pragma once

void guarantee_readdir_will_return_N_files_named(const char **files_names);
void guarantee_readdir_will_return_a_file_named(const char *file_name);
void guarantee_opendir_will_fail();
void guarantee_readdir_will_fail();
void reset_dirent_guarantees();

void verify_that_the_str_that_has_been_printed_is(const char *str, ...);
void reset_printing_buffer();

void guarantee_stat_will_populate_data_of_a_regular_type_file();
void guarantee_stat_will_populate_data_of_a_directory_type_file();
