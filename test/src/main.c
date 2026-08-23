#include "suites.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

int	main(void)
{
	if (CU_initialize_registry() != CUE_SUCCESS)
		return 1;

	register_file_entry_list_suite();
	register_directory_suite();
	register_renderer_suite();
	register_file_stats_suite();
	register_parsed_arguments_suite();
	register_application_suite();

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	const unsigned int failures = CU_get_number_of_failures();
	CU_cleanup_registry();

	return failures ? 1 : 0;
}
