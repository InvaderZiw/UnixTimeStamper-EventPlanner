#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timelib.h"

typedef struct {
    unsigned int start;
    unsigned int end; /* [start, end) in UTC */
} TInterval;

typedef struct {
    char name[11];
    TTimezone *tz;
    int interval_count;
    TInterval *intervals;
} TPerson;

int cmp_intervals(const void *a, const void *b) {
    const TInterval *i1 = (const TInterval *)a;
    const TInterval *i2 = (const TInterval *)b;

    if (i1->start != i2->start) {
        return (i1->start < i2->start) ? -1 : 1;
    }
    if (i1->end != i2->end) {
        return (i1->end < i2->end) ? -1 : 1;
    }
    return 0;
}

int cmp_uint(const void *a, const void *b) {
    unsigned int x = *(const unsigned int *)a;
    unsigned int y = *(const unsigned int *)b;

    if (x < y) {
        return -1;
    }
    if (x > y) {
        return 1;
    }
    return 0;
}

int cmp_persons_by_name(const void *a, const void *b) {
    const TPerson *p1 = (const TPerson *)a;
    const TPerson *p2 = (const TPerson *)b;
    return strcmp(p1->name, p2->name);
}

TTimezone *find_timezone(TTimezone *timezones, int T, const char *name) {
    int i;
    for (i = 0; i < T; i++) {
        if (strcmp(timezones[i].name, name) == 0) {
            return &timezones[i];
        }
    }
    return NULL;
}

void merge_person_intervals(TPerson *person) {
    int i;
    int new_count = 0;

    if (person->interval_count <= 1) {
        return;
    }

    qsort(person->intervals, person->interval_count, sizeof(TInterval), cmp_intervals);

    for (i = 0; i < person->interval_count; i++) {
        if (new_count == 0) {
            person->intervals[new_count++] = person->intervals[i];
        } else {
            TInterval *last = &person->intervals[new_count - 1];

            if (person->intervals[i].start <= last->end) {
                if (person->intervals[i].end > last->end) {
                    last->end = person->intervals[i].end;
                }
            } else {
                person->intervals[new_count++] = person->intervals[i];
            }
        }
    }

    person->interval_count = new_count;
}

int person_can_attend(const TPerson *person, unsigned int event_start,
                      unsigned int duration_seconds) {
    int i;
    unsigned int event_end = event_start + duration_seconds;

    for (i = 0; i < person->interval_count; i++) {
        if (person->intervals[i].start <= event_start &&
            person->intervals[i].end >= event_end) {
            return 1;
        }
    }

    return 0;
}

int main(void) {
    int T, P, i, j;
    TTimezone *timezones = NULL;
    TPerson *persons = NULL;
    unsigned int *candidates = NULL;
    int candidate_count = 0;
    int F, duration_hours;
    unsigned int duration_seconds;
    int found = 0;
    unsigned int event_start_utc = 0;

    if (scanf("%d", &T) != 1) {
        return 0;
    }

    timezones = (TTimezone *)malloc(T * sizeof(TTimezone));
    if (!timezones) {
        return 0;
    }

    for (i = 0; i < T; i++) {
        scanf("%4s %hhd", timezones[i].name, &timezones[i].utc_hour_difference);
    }

    scanf("%d", &P);

    persons = (TPerson *)malloc(P * sizeof(TPerson));
    if (!persons) {
        free(timezones);
        return 0;
    }

    for (i = 0; i < P; i++) {
        char tz_name[5];
        int K;

        scanf("%10s %4s %d", persons[i].name, tz_name, &K);
        persons[i].tz = find_timezone(timezones, T, tz_name);
        persons[i].interval_count = K;
        persons[i].intervals = NULL;

        if (K > 0) {
            persons[i].intervals = (TInterval *)malloc(K * sizeof(TInterval));
            if (!persons[i].intervals || !persons[i].tz) {
                int x;
                for (x = 0; x <= i; x++) {
                    free(persons[x].intervals);
                }
                free(persons);
                free(timezones);
                return 0;
            }
        } else if (!persons[i].tz) {
            int x;
            for (x = 0; x <= i; x++) {
                free(persons[x].intervals);
            }
            free(persons);
            free(timezones);
            return 0;
        }

        for (j = 0; j < K; j++) {
            int year, month, day, hour, len_hours;
            TDateTimeTZ dt;
            unsigned int start_ts;

            scanf("%d %d %d %d %d", &year, &month, &day, &hour, &len_hours);

            dt.date.year = (unsigned int)year;
            dt.date.month = (unsigned char)month;
            dt.date.day = (unsigned char)day;
            dt.time.hour = (unsigned char)hour;
            dt.time.min = 0;
            dt.time.sec = 0;
            dt.tz = persons[i].tz;

            start_ts = convertDateTimeTZToUnixTimestamp(dt);

            persons[i].intervals[j].start = start_ts;
            persons[i].intervals[j].end = start_ts + (unsigned int)len_hours * 3600u;
        }

        merge_person_intervals(&persons[i]);
        candidate_count += persons[i].interval_count;
    }

    scanf("%d %d", &F, &duration_hours);
    duration_seconds = (unsigned int)duration_hours * 3600u;

    candidates = NULL;
    if (candidate_count > 0) {
        candidates = (unsigned int *)malloc(candidate_count * sizeof(unsigned int));
        if (!candidates) {
            for (i = 0; i < P; i++) {
                free(persons[i].intervals);
            }
            free(persons);
            free(timezones);
            return 0;
        }
    }

    {
        int idx = 0;
        for (i = 0; i < P; i++) {
            for (j = 0; j < persons[i].interval_count; j++) {
                candidates[idx++] = persons[i].intervals[j].start;
            }
        }
    }

    if (candidate_count > 0) {
        qsort(candidates, candidate_count, sizeof(unsigned int), cmp_uint);

        for (i = 0; i < candidate_count; i++) {
            unsigned int candidate = candidates[i];
            int count = 0;

            if (i > 0 && candidates[i] == candidates[i - 1]) {
                continue;
            }

            for (j = 0; j < P; j++) {
                if (person_can_attend(&persons[j], candidate, duration_seconds)) {
                    count++;
                }
            }

            if (count >= F) {
                found = 1;
                event_start_utc = candidate;
                break;
            }
        }
    }

    if (!found) {
        printf("imposibil\n");
    } else {
        qsort(persons, P, sizeof(TPerson), cmp_persons_by_name);

        for (i = 0; i < P; i++) {
            printf("%s: ", persons[i].name);

            if (person_can_attend(&persons[i], event_start_utc, duration_seconds)) {
                int tz_index = (int)(persons[i].tz - timezones);
                TDateTimeTZ local_dt =
                    convertUnixTimestampToDateTimeTZ(event_start_utc, timezones, tz_index);
                printDateTimeTZ(local_dt);
            } else {
                printf("invalid\n");
            }
        }
    }

    free(candidates);
    for (i = 0; i < P; i++) {
        free(persons[i].intervals);
    }
    free(persons);
    free(timezones);

    return 0;
}