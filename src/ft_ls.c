#include "application.h"

int	main(const int argc, const char **argv)
{
	t_result *parsing_arguments_result = parse_arguments(argc - 1, &argv[1]);
	t_parsed_arguments *parsed_arguments = result_get_value(parsing_arguments_result);

	const t_ft_ls_error_code result = application_run(parsing_arguments_result);

	parsed_arguments_destroy(&parsed_arguments);
	result_destroy(&parsing_arguments_result);

	return result;
}
