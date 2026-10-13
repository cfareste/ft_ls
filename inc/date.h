#pragma once

#include <time.h>

typedef struct s_date
{
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
    time_t timestamp;
} t_date;

t_date date_from_time(time_t timestamp);
time_t date_get_current_time(void);
