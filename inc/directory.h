#pragma once

#include <dirent.h>

typedef struct s_dir_stream t_dir_stream;
typedef struct s_dir_entry t_dir_entry;

t_dir_stream *directory_open(const char *path);
struct dirent *directory_get_next_entry(t_dir_stream *stream);
int directory_close(t_dir_stream **dir_stream);
