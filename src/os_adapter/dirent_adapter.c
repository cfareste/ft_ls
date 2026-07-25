#include "os_adapter.h"

DIR *opendir_adapter(const char *path)
{
    return opendir(path);
}
