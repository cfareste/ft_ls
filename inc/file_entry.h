#pragma once

#include "file_stats.h"

typedef struct s_file_entry t_file_entry;
typedef struct s_file_entry_array t_file_entry_array;

t_file_entry *file_entry_create(const char *file_name);
t_file_entry_array *file_entry_array_create(void);
const char *file_entry_get_name(const t_file_entry *file_entry);
void file_entry_set_name(t_file_entry *entry, const char *name);
t_file_type file_entry_get_file_type(const t_file_entry *entry);
void file_entry_set_file_type(t_file_entry *entry, t_file_type file_type);
const t_file_entry *file_entry_array_get_at(const t_file_entry_array *file_entry_array, unsigned int index);
void file_entry_array_push(t_file_entry_array *array, t_file_entry *entry);
unsigned int file_entry_array_get_length(const t_file_entry_array *file_entry_array);
void file_entry_array_sort(const t_file_entry_array *file_entry_array);
void file_entry_destroy(t_file_entry **file_entry);
void file_entry_array_destroy(t_file_entry_array **file_entry_array);
