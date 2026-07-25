#pragma once

typedef struct s_file_data t_file_data;


t_file_data *file_data_create();
void file_data_destroy(t_file_data **_file_data);
