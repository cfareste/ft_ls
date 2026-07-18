#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

static void test_add(void)
{
	CU_ASSERT(2 + 3 == 5);
}

static void test_subtract(void)
{
	CU_ASSERT(7 - 4 == 3);
}

void register_math_suite(void)
{
	CU_pSuite suite;

	suite = CU_add_suite("math suite", NULL, NULL);
	if (suite != NULL)
	{
		CU_add_test(suite, "addition", test_add);
		CU_add_test(suite, "subtraction", test_subtract);
	}
}
