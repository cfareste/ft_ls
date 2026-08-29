#pragma once

typedef struct s_file_entry t_file_entry;
typedef struct s_file_entry_array t_file_entry_array;

t_file_entry *file_entry_create(const char *file_name);
t_file_entry_array *file_entry_array_create();
const char *file_entry_get_name(const t_file_entry *file_entry);
const char *file_entry_array_get_name(const t_file_entry_array *file_entry_array);
void file_entry_set_name(t_file_entry *entry, const char *name);
void file_entry_array_set_name(t_file_entry_array *file_entry_array, const char *name);
const t_file_entry *file_entry_array_get_at(const t_file_entry_array *file_entry_array, unsigned int index);
t_file_entry_array *file_entry_array_get_next(const t_file_entry_array *file_entry_array);
void file_entry_array_push_TEMP(t_file_entry_array *array, t_file_entry *entry);
void file_entry_array_push(t_file_entry_array **file_entry_array, t_file_entry_array *next);
unsigned int file_entry_array_get_length_TEMP(const t_file_entry_array *file_entry_array);
unsigned int file_entry_array_get_length(const t_file_entry_array *file_entry_array);
void file_entry_destroy(t_file_entry **file_entry);
void file_entry_array_destroy(t_file_entry_array **file_entry_array);
