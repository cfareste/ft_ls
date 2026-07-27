#include "file_data.h"

#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"

#define SUITE_NAME "file_data"

static t_file_data *sut;

static void initialize_file_data()
{
    sut = file_data_create();
}

static void cleanup_file_data()
{
    file_data_destroy(&sut);
}

static t_file_data *create_next_file_data(const char *file_name)
{
    t_file_data *next = file_data_create();
    file_data_set_name(next, file_name);

    return next;
}

static void should_be_created_correctly(void)
{
    initialize_file_data();

    CU_ASSERT_PTR_NOT_NULL(sut);

    cleanup_file_data();
}

static void should_be_destroyed_correctly(void)
{
    initialize_file_data();

    cleanup_file_data();

    CU_ASSERT_PTR_NULL(sut);
}

static void should_return_the_correct_length(void)
{
    initialize_file_data();

    file_data_add_entry(&sut, file_data_create());
    file_data_add_entry(&sut, file_data_create());

    CU_ASSERT_EQUAL(file_data_get_length(sut), 3);

    cleanup_file_data();
}

static void should_return_the_next_instance_correctly(void)
{
    initialize_file_data();

    const char *next_file_name = "next_file";
    t_file_data *next = create_next_file_data(next_file_name);
    file_data_add_entry(&sut, next);

    const char *file_name = file_data_get_name(file_data_get_next(sut));

    CU_ASSERT_STRING_EQUAL(file_name, next_file_name);

    cleanup_file_data();
}

void register_file_data_suite(void)
{
    const CU_pSuite suite = CU_add_suite(SUITE_NAME, NULL, NULL);

    if (suite != NULL)
    {
        CU_add_test(suite, "should_be_created_correctly", should_be_created_correctly);
        CU_add_test(suite, "should_be_destroyed_correctly", should_be_destroyed_correctly);
        CU_add_test(suite, "should_return_the_correct_length", should_return_the_correct_length);
        CU_add_test(suite, "should_return_the_next_instance_correctly", should_return_the_next_instance_correctly);
    }
}
