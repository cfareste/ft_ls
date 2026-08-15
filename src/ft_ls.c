#include "application.h"
#include "application_options.h"

int	main(const int argc, const char **argv)
{
	t_parsed_arguments *parsed_arguments = parse_arguments(argc - 1, &argv[1]);

	const int result = run_application(parsed_arguments);

	parsed_arguments_destroy(&parsed_arguments);

	return result;
}
