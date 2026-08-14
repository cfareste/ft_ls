#pragma once

#include <sys/stat.h>

#define FILE_DATA_SUCCESS (0)
#define FILE_DATA_COULD_NOT_RETRIEVE_DATA (-1)

int get_file_data(const char *file_path, struct stat *stats);
