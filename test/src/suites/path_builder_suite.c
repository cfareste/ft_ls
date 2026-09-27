#include <stdlib.h>
#include "suites.h"
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "path_builder.h"

#define SUITE_NAME "path_builder"

static char *sut = NULL;

static void test_teardown(void)
{
    free(sut);
    sut = NULL;
}

static void should_return_NULL_if_directory_path_is_NULL(void)
{
    sut = build_path(NULL, "valid_path");

    CU_ASSERT_PTR_NULL(sut);
}

static void should_return_NULL_if_directory_path_is_empty(void)
{
    sut = build_path("", "valid_path");

    CU_ASSERT_PTR_NULL(sut);
}

static void should_return_NULL_if_child_path_is_NULL(void)
{
    sut = build_path("valid_dir/", NULL);

    CU_ASSERT_PTR_NULL(sut);
}

static void should_return_NULL_if_child_path_is_empty(void)
{
    sut = build_path("valid_dir/", "");

    CU_ASSERT_PTR_NULL(sut);
}

static void should_return_the_built_path_without_adding_a_slash_if_directory_already_had_it(void)
{
    sut = build_path("dir/", "file");

    CU_ASSERT_STRING_EQUAL(sut, "dir/file");
}

static void should_return_the_built_path_adding_a_slash_if_directory_does_not_have_it(void)
{
    sut = build_path("dir", "file");

    CU_ASSERT_STRING_EQUAL(sut, "dir/file");
}

void register_path_builder_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, NULL, NULL, NULL, test_teardown);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_return_NULL_if_directory_path_is_NULL", should_return_NULL_if_directory_path_is_NULL);
        CU_add_test(suite, "should_return_NULL_if_directory_path_is_empty", should_return_NULL_if_directory_path_is_empty);
        CU_add_test(suite, "should_return_NULL_if_child_path_is_NULL", should_return_NULL_if_child_path_is_NULL);
        CU_add_test(suite, "should_return_NULL_if_child_path_is_empty", should_return_NULL_if_child_path_is_empty);
        CU_add_test(suite, "should_return_the_built_path_without_adding_a_slash_if_directory_already_had_it", should_return_the_built_path_without_adding_a_slash_if_directory_already_had_it);
        CU_add_test(suite, "should_return_the_built_path_adding_a_slash_if_directory_does_not_have_it", should_return_the_built_path_adding_a_slash_if_directory_does_not_have_it);
    }
}
