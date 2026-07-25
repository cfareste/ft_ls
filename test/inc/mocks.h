#pragma once

typedef struct DIR DIR;
struct dirent;

typedef enum e_failure_type
{
    NO_FAILURE = 0
} t_failure_type;

extern t_failure_type g_opendir_fail;

DIR *opendir_adapter(const char *path);
struct dirent *readdir_adapter(DIR *dir);
