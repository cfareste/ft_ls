#pragma once

typedef struct DIR {} DIR;
struct dirent
{
    char d_name[256];
};

typedef enum e_failure_type
{
    NO_FAILURE = 0
} t_failure_type;

extern t_failure_type g_opendir_fail;
extern t_failure_type g_readdir_fail;

DIR *opendir_adapter(const char *path);
struct dirent *readdir_adapter(DIR *dir);
void affirm_readdir_will_return_a_file_named(const char *path);
