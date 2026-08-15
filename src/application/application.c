#include "application.h"
#include "file_entry_list.h"
#include "scanner.h"
#include "renderer.h"
#include "error_codes.h"

int run_application(const t_parsed_arguments *parsed_arguments)
{
    t_file_entry_list *file_entry_list = scan(parsed_arguments_get_file_operands(parsed_arguments)[0]);

    render(file_entry_list);

    file_entry_list_destroy(&file_entry_list);

    return FT_LS_APPLICATION_SUCCESS;
}
