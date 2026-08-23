#pragma once

#include <sys/stat.h>

#define FILE_STATS_SUCCESS (0)
#define FILE_STATS_COULD_NOT_RETRIEVE_STATS (-1)

int file_stats_get(const char *file_path, struct stat *stats);
