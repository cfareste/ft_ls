#pragma once

typedef struct s_file_data t_file_data;


t_file_data *file_data_create();
void file_data_destroy(t_file_data **_file_data);
void file_data_add_entry(t_file_data *_file_data, t_file_data *_next);
unsigned int file_data_get_length(const t_file_data *_file_data);
