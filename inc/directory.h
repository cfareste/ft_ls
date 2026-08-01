#pragma once

#include <dirent.h>

DIR *directory_open(const char *path);
struct dirent *directory_get_next_entry(DIR *dir);
int directory_close(DIR *dir_stream);
