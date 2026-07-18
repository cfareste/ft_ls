#include "suites.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

int	main(void)
{
	unsigned int failures;

	if (CU_initialize_registry() != CUE_SUCCESS)
		return 1;

	register_math_suite();
	register_string_suite();

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	failures = CU_get_number_of_failures();
	CU_cleanup_registry();

	return failures ? 1 : 0;
}
