#pragma once

struct dirent
{
    char d_name[256];
};

typedef struct DIR
{
    struct dirent entry;
} DIR;

typedef enum e_failure_type
{
    NO_FAILURE = 0
} t_failure_type;

extern t_failure_type g_opendir_fail;
extern t_failure_type g_readdir_fail;
extern t_failure_type g_closedir_fail;

void affirm_readdir_will_return_N_files_named(const char *file_name, unsigned int num);
void affirm_readdir_will_return_a_file_named(const char *file_name);
