#pragma once

typedef struct s_file_entry t_file_entry;
typedef struct s_file_entry_array t_file_entry_array;

t_file_entry *file_entry_create(const char *file_name);
t_file_entry_array *file_entry_array_create(void);
const char *file_entry_get_name(const t_file_entry *file_entry);
void file_entry_set_name(t_file_entry *entry, const char *name);
const t_file_entry *file_entry_array_get_at(const t_file_entry_array *file_entry_array, unsigned int index);
void file_entry_array_push(t_file_entry_array *array, t_file_entry *entry);
unsigned int file_entry_array_get_length(const t_file_entry_array *file_entry_array);
void file_entry_array_sort(const t_file_entry_array *file_entry_array);
void file_entry_destroy(t_file_entry **file_entry);
void file_entry_array_destroy(t_file_entry_array **file_entry_array);
