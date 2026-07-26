#pragma once

typedef struct dir_stream t_dir_stream;
typedef struct dir_entry t_dir_entry;

t_dir_stream *opendir_adapter(const char *path);

t_dir_entry *readdir_adapter(t_dir_stream *dir);
int closedir_adapter(t_dir_stream **dir_stream);

void dir_entry_destroy(t_dir_entry **dir_entry);
const char *dir_entry_get_name(const t_dir_entry *dir_entry);
short dir_entry_is_null(const t_dir_entry *dir_entry);
