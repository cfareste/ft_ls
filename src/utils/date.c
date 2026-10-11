#include <time.h>
#include "date.h"
#include "libft.h"

enum e_ctime_layout
{
    MONTH_OFFSET = 4,
    MONTH_LENGTH = 3,
    DAY_OFFSET = 8,
    HOUR_OFFSET = 11,
    MINUTE_OFFSET = 14,
    SECOND_OFFSET = 17,
    YEAR_OFFSET = 20,
    MONTH_COUNT = 12
};

static int get_month_number(const char *month)
{
    static const char *months[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    for (int i = 0; i < MONTH_COUNT; i++)
    {
        if (ft_strncmp(month, months[i], MONTH_LENGTH) == EQUAL_STRINGS)
            return i + 1;
    }
    return 0;
}


t_date date_from_time(const time_t timestamp)
{
    t_date date = {0};
    const char *str = ctime(&timestamp);

    if (str == NULL)
        return date;

    date.day = ft_atoi(&str[DAY_OFFSET]);
    date.month = get_month_number(&str[MONTH_OFFSET]);
    date.year = ft_atoi(&str[YEAR_OFFSET]);
    date.hour = ft_atoi(&str[HOUR_OFFSET]);
    date.minute = ft_atoi(&str[MINUTE_OFFSET]);
    date.second = ft_atoi(&str[SECOND_OFFSET]);
    date.timestamp = timestamp;

    return date;
}