#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timelib.h"
#define hour_in_sec 3600
#define min_in_sec 60
#define h 24
#define year_in_sec 31536000
#define leapyear_in_sec 31622400
#define day_in_sec 86400
#define month_in_sec 2628000
#define MAXV 12
#define MONTH_WITH_31_DAYS 31
#define MONTH_WITH_30_DAYS 30
#define FEBRUARY 28
#define FEBRUARYLEAP 29
#define NRYEARS 500
#define START_TIMESTAMP 1970
#define NOLEAPYEAR 365
#define LEAPYEAR 366
#define IANUARIE 1
#define FEBRUARIE 2
#define MARTIE 3
#define APRILIE 4
#define MAI 5
#define IUNIE 6
#define IULIE 7
#define AUGUST 8
#define SEPTEMBRIE 9
#define OCTOMBRIE 10
#define NOIEMBRIE 11
#define DECEMBRIE 12
#define NONDAY 32
#define LEAPCOND1 100
#define LEAPCOND2 400
#define NONMONTH 13
#define MAXL 12
#define MAXC 20

// Task 1
TTime convertUnixTimestampToTime(unsigned int timestamp) {
    TTime result = {0};
    unsigned int hour_step_1 = timestamp / hour_in_sec;
    result.min = (timestamp - (hour_step_1 * hour_in_sec)) / min_in_sec;
    result.sec = timestamp - hour_step_1 * hour_in_sec - result.min * min_in_sec;
    result.hour = hour_step_1 % h;
    result.min = result.min % min_in_sec;
    result.sec = result.sec % min_in_sec;
    return result;
}

// Task 2
TDate convertUnixTimestampToDateWithoutLeapYears(unsigned int timestamp) {
    TDate result = {0};
    unsigned int year_step_1 = 0, month_step_1 = 0, day_step_1 = 0;
    year_step_1 = timestamp / year_in_sec;
    result.year = year_step_1;
    unsigned int T = timestamp - (result.year * year_in_sec);
    unsigned int DAY = T / day_in_sec;
    unsigned int V[MAXV] = {MONTH_WITH_31_DAYS, FEBRUARY, MONTH_WITH_31_DAYS, MONTH_WITH_30_DAYS, \
    MONTH_WITH_31_DAYS, MONTH_WITH_30_DAYS, MONTH_WITH_31_DAYS, MONTH_WITH_31_DAYS, MONTH_WITH_30_DAYS, \
    MONTH_WITH_31_DAYS, MONTH_WITH_30_DAYS, MONTH_WITH_31_DAYS};
    unsigned int DAY_COUNTER = 0;
    for (int i = 0; i < MAXV; i++) {
        if (DAY >= DAY_COUNTER && DAY < DAY_COUNTER + V[i]) {
            month_step_1 = i + 1;
            day_step_1 = 1 + (DAY - DAY_COUNTER);
            break;
        }
        DAY_COUNTER += V[i];
    }
    result.year += START_TIMESTAMP;
    result.month = month_step_1;
    result.day = day_step_1;
    return result;
}

//  Task 3
TDate convertUnixTimestampToDate(unsigned int timestamp) {
    TDate result = {0};
    unsigned int year_step_1 = 0, month_step_1 = 0, day_step_1 = 0;
    unsigned int DAY = 0;
    DAY = timestamp / day_in_sec;
    unsigned int V[NRYEARS] = {0};
    for (int i = 0; i < NRYEARS - 1; i++) {
        if ((i + 2) % 4 == 0) {
            V[i + 1] = V[i] + LEAPYEAR;
        }
        if (!((i + 2) % 4 == 0))
            V[i + 1] = V[i] + NOLEAPYEAR;
    }
    for (int i = 0; i < NRYEARS - 1; i++) {
        if ((DAY >= V[i]) && (DAY < V[i + 1])) {
            year_step_1 = (i + START_TIMESTAMP);
            DAY = DAY - V[i];
            break;
        }
    }
    if (year_step_1 % 4 != 0) {
        unsigned int U[MAXV] = {MONTH_WITH_31_DAYS, FEBRUARY, MONTH_WITH_31_DAYS, MONTH_WITH_30_DAYS, \
        MONTH_WITH_31_DAYS, MONTH_WITH_30_DAYS, MONTH_WITH_31_DAYS, MONTH_WITH_31_DAYS, MONTH_WITH_30_DAYS, \
        MONTH_WITH_31_DAYS, MONTH_WITH_30_DAYS, MONTH_WITH_31_DAYS};
        unsigned int DAY_COUNTER = 0;
        for (int i = 0; i < MAXV; i++) {
            if (DAY >= DAY_COUNTER && DAY < DAY_COUNTER + U[i]) {
                month_step_1 = i + 1;
                day_step_1 = 1 + (DAY - DAY_COUNTER);
                break;
            }
            DAY_COUNTER += U[i];
        }
    }
    if (!(year_step_1 % 4 != 0)) {
unsigned int U[MAXV] = {MONTH_WITH_31_DAYS, FEBRUARYLEAP, MONTH_WITH_31_DAYS, MONTH_WITH_30_DAYS, \
MONTH_WITH_31_DAYS, MONTH_WITH_30_DAYS, MONTH_WITH_31_DAYS, MONTH_WITH_31_DAYS, MONTH_WITH_30_DAYS, \
MONTH_WITH_31_DAYS, MONTH_WITH_30_DAYS, MONTH_WITH_31_DAYS};
        unsigned int DAY_COUNTER = 0;
        for (int i = 0; i < MAXV; i++) {
            if (DAY >= DAY_COUNTER && DAY < DAY_COUNTER + U[i]) {
                month_step_1 = i + 1;
                day_step_1 = 1 + (DAY - DAY_COUNTER);
                break;
            }
            DAY_COUNTER += U[i];
        }
    }
    result.year = year_step_1;
    result.month = month_step_1;
    result.day = day_step_1;
    return result;
}

TDateTimeTZ convertUnixTimestampToDateTimeTZ(unsigned int timestamp, TTimezone *timezones, int timezone_index) {
    TDateTimeTZ result = {0};
    int auxhour = 0;
    result.date = convertUnixTimestampToDate(timestamp);
    result.time = convertUnixTimestampToTime(timestamp);
    result.tz = &timezones[timezone_index];
    auxhour = result.time.hour + timezones[timezone_index].utc_hour_difference;
    result.time.hour += timezones[timezone_index].utc_hour_difference;
    if (auxhour > 0) {
        if (result.time.hour >= h) {
            result.date.day++;
            result.time.hour = result.time.hour - h;
        }
        if (result.date.day == MONTH_WITH_31_DAYS && (result.date.month == APRILIE || result.date.month == IUNIE ||
            result.date.month == SEPTEMBRIE || result.date.month == NOIEMBRIE)) {
            result.date.day = 1;
            result.date.month++;
        }
        if (result.date.day == NONDAY && (result.date.month != FEBRUARIE && result.date.month != \
            APRILIE && result.date.month != IUNIE && result.date.month != SEPTEMBRIE && result.date.\
            month != NOIEMBRIE)) {
            result.date.day = 1;
            result.date.month++;
            if (result.date.month == NONMONTH) {
                result.date.year++;
                result.date.month = 1;
            }
            if (((result.date.year % 4 == 0 && result.date.year % LEAPCOND1 != 0) || \
                result.date.year % LEAPCOND2 == 0) && \
                result.date.month == FEBRUARIE && result.date.day == MONTH_WITH_30_DAYS) {
                result.date.day = 1;
                result.date.month++;
            }
            if (!((result.date.year % 4 == 0 && result.date.year % LEAPCOND1 != 0) || \
                   result.date.year % LEAPCOND2 == 0) && \
                   result.date.month == FEBRUARIE && result.date.day == FEBRUARYLEAP) {
                result.date.day = 1;
                result.date.month++;
            }
        }
    }
    if (auxhour < 0) {
        result.date.day--;
        result.time.hour = h + auxhour;
        if (result.date.day == 0) {
            if (result.date.month == MAI || result.date.month == IULIE || \
            result.date.month == OCTOMBRIE || result.date.month == DECEMBRIE) {
                result.date.day = MONTH_WITH_30_DAYS;
                result.date.month--;
            }
            if (!(result.date.month == MAI || result.date.month == IULIE || \
            result.date.month == OCTOMBRIE || result.date.month == DECEMBRIE)) {
                result.date.day = MONTH_WITH_31_DAYS;
                result.date.month--;
            }
            if (((result.date.year % 4 == 0 && result.date.year % LEAPCOND1 != 0) || \
                result.date.year % LEAPCOND2 == 0) && \
                result.date.month == MARTIE) {
                result.date.day = FEBRUARYLEAP;
                result.date.month--;
            }
            if (!((result.date.year % 4 == 0 && result.date.year % LEAPCOND1 != 0) || \
                   result.date.year % LEAPCOND2 == 0) && \
                   result.date.month == MARTIE) {
                result.date.day = FEBRUARY;
                result.date.month--;
            }
        }
    }
    return result;
}

// Task 5
unsigned int convertDateTimeTZToUnixTimestamp(TDateTimeTZ temp) {
    unsigned int DAYS = 0, result = 0;
    for (int i = START_TIMESTAMP; i < temp.date.year; i++) {
        if ((i % 4 == 0 && i % LEAPCOND1 != 0) || i % LEAPCOND2 == 0)
            DAYS += LEAPYEAR;
        else
            DAYS += NOLEAPYEAR;
    }
    for (int i = 1; i < temp.date.month; i++) {
        if (i == 2 && ((temp.date.year % 4 == 0 && temp.date.year % LEAPCOND1 != 0) || temp.date.year % LEAPCOND2 == 0))
            DAYS += FEBRUARYLEAP;
        if (i == 2 && (!((temp.date.year % 4 == 0 && temp.date.year % LEAPCOND1 != 0) || \
         temp.date.year % LEAPCOND2 == 0)))
            DAYS += FEBRUARY;
        if (i == APRILIE || i == IUNIE || i == SEPTEMBRIE || i == NOIEMBRIE)
            DAYS += MONTH_WITH_30_DAYS;
        if (i == IANUARIE || i == MARTIE || i == MAI || i == IULIE || i == AUGUST || i == OCTOMBRIE)
            DAYS += MONTH_WITH_31_DAYS;
    }

    DAYS += temp.date.day;
    result = (DAYS * day_in_sec) + ((temp.time.hour - temp.tz->utc_hour_difference) * \
    hour_in_sec) + (temp.time.min * min_in_sec) + (temp.time.sec) - day_in_sec;
    return result;
}

// Task 6
void printDateTimeTZ(TDateTimeTZ datetimetz) {
    char luni[MAXL][MAXC] = {"ianuarie", "februarie", "martie", "aprilie", "mai", "iunie", "iulie", \
    "august", "septembrie", "octombrie", "noiembrie", "decembrie"};
    printf("%02u %s %u, %02u:%02u:%02u %s (UTC%+i)\n", datetimetz.date.day, luni[datetimetz.date.month - 1], \
            datetimetz.date.year, datetimetz.time.hour, datetimetz.time.min, datetimetz.time.sec, datetimetz. \
            tz->name, datetimetz.tz->utc_hour_difference);
}
