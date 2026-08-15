#include "application.h"
#include "application_options.h"

int	main(const int argc, const char **argv)
{
	t_ft_ls_options *options = ft_ls_options_get(argc, argv);

	const int result = run_application(options);

	ft_ls_options_destroy(&options);

	return result;
}
