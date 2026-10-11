#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "date.h"
#include "suites.h"

#define SUITE_NAME "date"
#define UTC_TIMEZONE "UTC0"

static char *original_timezone;
static int had_original_timezone;

static int set_up_utc_timezone(void)
{
    const char *timezone_local = getenv("TZ");

    had_original_timezone = timezone_local != NULL;
    original_timezone = NULL;
    if (had_original_timezone)
    {
        original_timezone = strdup(timezone_local);
        if (original_timezone == NULL)
        {
            CU_FAIL("Could not save the current timezone");
            return -1;
        }
    }
    if (setenv("TZ", UTC_TIMEZONE, 1) != 0)
    {
        free(original_timezone);
        original_timezone = NULL;
        CU_FAIL("Could not set the test timezone");
        return -1;
    }
    tzset();
    return 0;
}

static int restore_timezone(void)
{
    int result;

    if (had_original_timezone)
        result = setenv("TZ", original_timezone, 1);
    else
        result = unsetenv("TZ");
    tzset();
    free(original_timezone);
    original_timezone = NULL;
    had_original_timezone = 0;
    CU_ASSERT_EQUAL(result, 0);
    return 0;
}

static void should_convert_unix_epoch(void)
{
    const time_t timestamp = 0;
    const t_date date = date_from_time(timestamp);

    CU_ASSERT_EQUAL(date.day, 1);
    CU_ASSERT_EQUAL(date.month, 1);
    CU_ASSERT_EQUAL(date.year, 1970);
    CU_ASSERT_EQUAL(date.hour, 0);
    CU_ASSERT_EQUAL(date.minute, 0);
    CU_ASSERT_EQUAL(date.second, 0);
    CU_ASSERT_EQUAL(date.timestamp, timestamp);
}

static void should_convert_leap_day_with_two_digit_day(void)
{
    const time_t timestamp = 951868798;
    const t_date date = date_from_time(timestamp);

    CU_ASSERT_EQUAL(date.day, 29);
    CU_ASSERT_EQUAL(date.month, 2);
    CU_ASSERT_EQUAL(date.year, 2000);
    CU_ASSERT_EQUAL(date.hour, 23);
    CU_ASSERT_EQUAL(date.minute, 59);
    CU_ASSERT_EQUAL(date.second, 58);
    CU_ASSERT_EQUAL(date.timestamp, timestamp);
}

static void should_convert_single_digit_day_and_month(void)
{
    const time_t timestamp = 1725670923;
    const t_date date = date_from_time(timestamp);

    CU_ASSERT_EQUAL(date.day, 7);
    CU_ASSERT_EQUAL(date.month, 9);
    CU_ASSERT_EQUAL(date.year, 2024);
    CU_ASSERT_EQUAL(date.hour, 1);
    CU_ASSERT_EQUAL(date.minute, 2);
    CU_ASSERT_EQUAL(date.second, 3);
    CU_ASSERT_EQUAL(date.timestamp, timestamp);
}

static void should_convert_year_boundary(void)
{
    const time_t timestamp = 1735648496;
    const t_date date = date_from_time(timestamp);

    CU_ASSERT_EQUAL(date.day, 31);
    CU_ASSERT_EQUAL(date.month, 12);
    CU_ASSERT_EQUAL(date.year, 2024);
    CU_ASSERT_EQUAL(date.hour, 12);
    CU_ASSERT_EQUAL(date.minute, 34);
    CU_ASSERT_EQUAL(date.second, 56);
    CU_ASSERT_EQUAL(date.timestamp, timestamp);
}

static void should_map_every_month_abbreviation(void)
{
    static const time_t timestamps[] = {
        1704112496, 1706790896, 1709296496, 1711974896,
        1714566896, 1717245296, 1719837296, 1722515696,
        1725194096, 1727786096, 1730464496, 1733056496
    };

    for (int month = 1; month <= 12; month++)
    {
        const t_date date = date_from_time(timestamps[month - 1]);

        CU_ASSERT_EQUAL(date.day, 1);
        CU_ASSERT_EQUAL(date.month, month);
        CU_ASSERT_EQUAL(date.year, 2024);
        CU_ASSERT_EQUAL(date.hour, 12);
        CU_ASSERT_EQUAL(date.minute, 34);
        CU_ASSERT_EQUAL(date.second, 56);
        CU_ASSERT_EQUAL(date.timestamp, timestamps[month - 1]);
    }
}

void register_date_suite(void)
{
    const CU_pSuite suite = CU_add_suite_with_setup_and_teardown(SUITE_NAME, set_up_utc_timezone, restore_timezone, NULL, NULL);

    if (suite == NULL)
        return ;

    CU_add_test(suite, "should_convert_unix_epoch", should_convert_unix_epoch);
    CU_add_test(suite, "should_convert_leap_day_with_two_digit_day", should_convert_leap_day_with_two_digit_day);
    CU_add_test(suite, "should_convert_single_digit_day_and_month", should_convert_single_digit_day_and_month);
    CU_add_test(suite, "should_convert_year_boundary", should_convert_year_boundary);
    CU_add_test(suite, "should_map_every_month_abbreviation", should_map_every_month_abbreviation);
}
