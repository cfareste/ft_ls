#include "application.h"
#include "file_entry_list.h"
#include "scanner.h"
#include "renderer.h"

int run_application()
{
    t_file_entry_list *file_entry_list = scan(".");

    render(file_entry_list);

    file_entry_list_destroy(&file_entry_list);

    return FT_LS_APPLICATION_SUCCESS;
}
