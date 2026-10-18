#pragma once

#include "file_stats.h"

typedef struct s_file_entry t_file_entry;
typedef struct s_file_entry_array t_file_entry_array;

t_file_entry *file_entry_create(const char *file_name);
t_file_entry_array *file_entry_array_create(void);
const char *file_entry_get_name(const t_file_entry *file_entry);
void file_entry_set_name(t_file_entry *entry, const char *name);
long file_entry_get_blocks(const t_file_entry *entry);
void file_entry_set_blocks(t_file_entry *entry, long blocks);
t_permissions file_entry_get_permissions(const t_file_entry *entry);
void file_entry_set_permissions(t_file_entry *entry, t_permissions permissions);
t_file_type file_entry_get_file_type(const t_file_entry *entry);
void file_entry_set_file_type(t_file_entry *entry, t_file_type file_type);
unsigned long file_entry_get_link_count(const t_file_entry *entry);
void file_entry_set_link_count(t_file_entry *entry, unsigned long link_count);
const char *file_entry_get_author(const t_file_entry *entry);
void file_entry_set_author(t_file_entry *entry, const char *author);
const char *file_entry_get_group(const t_file_entry *entry);
void file_entry_set_group(t_file_entry *entry, const char *group);
long file_entry_get_size(const t_file_entry *entry);
void file_entry_set_size(t_file_entry *entry, long size);
t_device file_entry_get_device(const t_file_entry *entry);
void file_entry_set_device(t_file_entry *entry, t_device device);
t_date file_entry_get_modification_date(const t_file_entry *entry);
void file_entry_set_modification_date(t_file_entry *entry, t_date modification_date);
const char *file_entry_get_link_target(const t_file_entry *entry);
void file_entry_set_link_target(t_file_entry *entry, const char *target);
const t_file_entry *file_entry_array_get_at(const t_file_entry_array *file_entry_array, unsigned int index);
void file_entry_array_push(t_file_entry_array *array, t_file_entry *entry);
unsigned int file_entry_array_get_length(const t_file_entry_array *file_entry_array);
unsigned long file_entry_array_get_biggest_link_count(const t_file_entry_array *file_entry_array);
void file_entry_array_sort(const t_file_entry_array *file_entry_array);
void file_entry_destroy(t_file_entry **file_entry);
void file_entry_array_destroy(t_file_entry_array **file_entry_array);
