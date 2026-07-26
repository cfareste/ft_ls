#include "libft.h"
#include "scanner.h"

int	main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	t_file_data *file_list = scan(".");

	t_file_data *temp = file_list;
	while (temp)
	{
		ft_printf("%s\n", file_data_get_name(temp));
		temp = file_data_get_next(temp);
	}

	file_data_destroy(&file_list);
	return 0;
}
