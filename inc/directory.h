#pragma once

typedef struct s_dir_stream t_dir_stream;
typedef struct s_dir_entry t_dir_entry;

t_dir_stream *directory_open(const char *path);
t_dir_entry *directory_get_next(const t_dir_stream *dir_stream);
const char *directory_get_entry_name(const t_dir_entry *dir_entry);
int directory_is_entry_empty(const t_dir_entry *dir_entry);
void directory_destroy_entry(t_dir_entry **dir_entry);
int directory_close(t_dir_stream **dir_stream);
