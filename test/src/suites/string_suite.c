#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

static void test_concat(void)
{
	CU_ASSERT_STRING_EQUAL("hello world", "hello world");
}

static void test_length(void)
{
	CU_ASSERT(11 == 11);
}

void register_string_suite(void)
{
	CU_pSuite suite;

	suite = CU_add_suite("string suite", NULL, NULL);
	if (suite != NULL)
	{
		CU_add_test(suite, "concat", test_concat);
		CU_add_test(suite, "length", test_length);
	}
}
