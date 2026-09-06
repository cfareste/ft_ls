#pragma once

#include <dirent.h>

typedef struct s_dir_stream t_dir_stream;
typedef struct s_dir_entry t_dir_entry;

DIR *directory_open(const char *path);
struct dirent *directory_get_next_entry(DIR *dir);
int directory_close(DIR **dir_stream);
