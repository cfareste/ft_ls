#pragma once

typedef struct s_file_entry_list t_file_entry_list;

t_file_entry_list *file_entry_list_create(const char *file_name);
const char *file_entry_list_get_name(const t_file_entry_list *file_entry_list);
void file_entry_list_set_name(t_file_entry_list *file_entry_list, const char *name);
t_file_entry_list *file_entry_list_get_next(const t_file_entry_list *file_entry_list);
void file_entry_list_add_entry(t_file_entry_list **file_entry_list, t_file_entry_list *next);
unsigned int file_entry_list_get_length(const t_file_entry_list *file_entry_list);
void file_entry_list_destroy(t_file_entry_list **file_entry_list);
