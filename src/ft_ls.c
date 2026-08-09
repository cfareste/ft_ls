#include "scanner.h"
#include "renderer.h"

int	main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	t_file_data *file_list = scan(".");

	render(file_list);

	file_data_destroy(&file_list);

	return 0;
}
