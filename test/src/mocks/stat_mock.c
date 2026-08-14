#include <sys/stat.h>
#include "libft.h"

static unsigned int mode = 0;

int stat_mock(const char *restrict pathname, struct stat *restrict statbuf)
{
    (void) pathname;
    statbuf->st_mode = mode;

    return 0;
}

void guarantee_stat_will_populate_data_of_a_regular_type_file()
{
    mode = S_IFREG;
}

void guarantee_stat_will_populate_data_of_a_directory_type_file()
{
    mode = S_IFDIR;
}

void reset_stat_guarantees()
{
    mode = 0;
}
