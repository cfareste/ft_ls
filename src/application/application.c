#include "application.h"
#include "file_entry_list.h"
#include "scanner.h"
#include "renderer.h"

int run_application(const t_ft_ls_options *options)
{
    t_file_entry_list *file_entry_list = scan(ft_ls_options_get_file_parameters(options)[0]);

    render(file_entry_list);

    file_entry_list_destroy(&file_entry_list);

    return FT_LS_APPLICATION_SUCCESS;
}
