#pragma once

extern short g_opendir_fail;
extern short g_readdir_fail;

void affirm_readdir_will_return_N_files_named(const char *file_name, unsigned int num);
void affirm_readdir_will_return_a_file_named(const char *file_name);
void reset_readdir_affirmations();
