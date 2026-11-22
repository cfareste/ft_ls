#include <sys/ioctl.h>
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "render_strategies.h"

#define DEFAULT_TERMINAL_WIDTH 80
#define MINIMUM_COLUMN_WIDTH 3
#define COLUMN_SEPARATOR_WIDTH 2

typedef struct s_column_info
{
    int valid;
    size_t line_len;
    size_t *col_arr;
} t_column_info;

static size_t get_terminal_width(void)
{
    struct winsize terminal_size = { 0 };

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal_size) != 0 || terminal_size.ws_col == 0)
        return DEFAULT_TERMINAL_WIDTH;
    return terminal_size.ws_col;
}

static size_t get_maximum_column_count(const unsigned int file_count, const size_t terminal_width)
{
    size_t maximum_column_count = terminal_width / MINIMUM_COLUMN_WIDTH;

    if (maximum_column_count > file_count)
        maximum_column_count = file_count;
    if (maximum_column_count == 0)
        maximum_column_count = 1;
    return maximum_column_count;
}

static size_t get_row_count(const unsigned int file_count, const size_t column_count)
{
    return file_count / column_count + (file_count % column_count != 0);
}

static void initialize_column_infos(t_column_info *column_infos, const size_t maximum_column_count)
{
    for (size_t column_count = 1; column_count <= maximum_column_count; column_count++)
    {
        t_column_info *column_info = &column_infos[column_count - 1];

        column_info->valid = 1;
        column_info->line_len = COLUMN_SEPARATOR_WIDTH * (column_count - 1);
        column_info->col_arr = ft_safe_calloc(column_count, sizeof(size_t));
    }
}

static void update_column_infos(t_column_info *column_infos, const size_t maximum_column_count, const unsigned int file_count, const t_file_entry_array *file_entry_array, const size_t terminal_width)
{
    for (unsigned int file_index = 0; file_index < file_count; file_index++)
    {
        const t_file_entry *entry = file_entry_array_get_at(file_entry_array, file_index);
        const size_t name_width = ft_strlen(file_entry_get_name(entry));

        for (size_t candidate = 0; candidate < maximum_column_count; candidate++)
        {
            t_column_info *column_info = &column_infos[candidate];
            const size_t column_count = candidate + 1;
            const size_t row_count = get_row_count(file_count, column_count);
            const size_t column_index = file_index / row_count;

            if (!column_info->valid || name_width <= column_info->col_arr[column_index])
                continue;

            column_info->line_len += name_width - column_info->col_arr[column_index];
            column_info->col_arr[column_index] = name_width;
            if (column_info->line_len > terminal_width)
                column_info->valid = 0;
        }
    }
}

static void destroy_column_infos(t_column_info *column_infos, const size_t maximum_column_count)
{
    for (size_t i = 0; i < maximum_column_count; i++)
        free(column_infos[i].col_arr);
    free(column_infos);
}

static t_column_info *get_winning_column_info(t_column_info *column_infos, const size_t maximum_column_count)
{
    for (size_t i = maximum_column_count; i > 0; i--)
    {
        if (column_infos[i - 1].valid)
            return &column_infos[i - 1];
    }
    return NULL;
}

static size_t get_last_column_with_entry(const unsigned int file_count, const size_t row_index, const size_t row_count, const size_t column_count)
{
    size_t last_column = column_count;

    while (last_column > 0)
    {
        const size_t column_index = last_column - 1;

        if (column_index * row_count + row_index < file_count)
            return column_index;
        last_column--;
    }
    return 0;
}

static void print_spaces(const size_t count)
{
    for (size_t i = 0; i < count; i++)
        ft_printf(" ");
}

static void print_entries_in_columns(const t_file_entry_array *file_entry_array, const unsigned int file_count, const size_t column_count, const size_t *column_widths)
{
    const size_t row_count = get_row_count(file_count, column_count);

    for (size_t row = 0; row < row_count; row++)
    {
        const size_t last_column = get_last_column_with_entry(file_count, row, row_count, column_count);

        for (size_t column = 0; column <= last_column; column++)
        {
            const size_t file_index = column * row_count + row;
            const t_file_entry *entry = file_entry_array_get_at(file_entry_array, file_index);
            const char *name = file_entry_get_name(entry);

            ft_printf("%s", name);
            if (column < last_column)
                print_spaces(column_widths[column] - ft_strlen(name) + COLUMN_SEPARATOR_WIDTH);
        }
        ft_printf("\n");
    }
}

void multi_column_strategy(const t_file_entry_array *file_entry_array)
{
    if (file_entry_array == NULL)
        return;

    const unsigned int file_count = file_entry_array_get_length(file_entry_array);
    if (file_count == 0)
        return;

    const size_t terminal_width = get_terminal_width();
    const size_t maximum_column_count = get_maximum_column_count(file_count, terminal_width);
    t_column_info *column_infos = ft_safe_calloc(maximum_column_count, sizeof(t_column_info));

    initialize_column_infos(column_infos, maximum_column_count);
    update_column_infos(column_infos, maximum_column_count, file_count, file_entry_array, terminal_width);

    t_column_info *winning_column_info = get_winning_column_info(column_infos, maximum_column_count);
    if (winning_column_info == NULL)
        single_column_strategy(file_entry_array);
    else
        print_entries_in_columns(file_entry_array, file_count, (size_t)(winning_column_info - column_infos) + 1, winning_column_info->col_arr);

    destroy_column_infos(column_infos, maximum_column_count);
}
