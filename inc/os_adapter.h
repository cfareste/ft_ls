#pragma once

typedef struct dir_stream t_dir_stream;
typedef struct dir_entry t_dir_entry;

t_dir_stream *opendir_adapter(const char *path);
const t_dir_entry *readdir_adapter(t_dir_stream *dir);
int closedir_adapter(t_dir_stream **dir_stream);
const char *dir_entry_get_name(const t_dir_entry *dir_entry);
