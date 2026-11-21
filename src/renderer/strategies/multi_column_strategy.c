#include <sys/ioctl.h>
#include <unistd.h>
#include "render_strategies.h"

void multi_column_strategy(const t_file_entry_array *file_entry_array)
{
    struct winsize terminal_size = { 0 };

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal_size);
    single_column_strategy(file_entry_array);
}
