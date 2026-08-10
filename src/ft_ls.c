#include "scanner.h"
#include "renderer.h"

int	main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	t_file_entry_list *file_entry_list = scan(".");

	render(file_entry_list);

	file_entry_list_destroy(&file_entry_list);

	return 0;
}
