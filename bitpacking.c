#include <stdio.h>
#include <stdlib.h>
#include "timelib.h"

#define BITI_ZI 5
#define BITI_LUNA 4
#define BITI_AN 6
#define BITI_DATA 15
#define AN_BAZA 1970
#define TASK7 7
#define TASK8 8
#define NR_BITI_UINT 32

typedef struct {
    unsigned int raw;
    TDate date;
} PackedDate;

static const char *luni[] = {
    "ianuarie", "februarie", "martie", "aprilie",
    "mai", "iunie", "iulie", "august",
    "septembrie", "octombrie", "noiembrie", "decembrie"
};

int cmp_dates(const void *a, const void *b) {
    const TDate *d1 = (const TDate *)a;
    const TDate *d2 = (const TDate *)b;

    if (d1->year != d2->year) {
        return (d1->year < d2->year) ? -1 : 1;
    }
    if (d1->month != d2->month) {
        return (d1->month < d2->month) ? -1 : 1;
    }
    if (d1->day != d2->day) {
        return (d1->day < d2->day) ? -1 : 1;
    }
    return 0;
}

TDate decode_date_15bits(unsigned int value) {
    TDate d;
    d.day = value & ((1u << BITI_ZI) - 1u);
    d.month = (value >> BITI_ZI) & ((1u << BITI_LUNA) - 1u);
    d.year = ((value >> (BITI_ZI + BITI_LUNA)) & ((1u << BITI_AN) - 1u)) + AN_BAZA;
    return d;
}

int parity_uint(unsigned int x) {
    int cnt = 0;
    while (x) {
        cnt += (x & 1u);
        x >>= 1;
    }
    return cnt % 2;
}

unsigned int extract_15_bits(const unsigned int *data, unsigned int date_index) {
    unsigned int start_bit = date_index * BITI_DATA;
    unsigned int first_int = start_bit / NR_BITI_UINT;
    unsigned int offset = start_bit % NR_BITI_UINT;

    if (offset <= NR_BITI_UINT - BITI_DATA) {
        return (data[first_int] >> offset) & ((1u << BITI_DATA) - 1u);
    } else {
        unsigned int bits_first = NR_BITI_UINT - offset;
        unsigned int bits_second = BITI_DATA - bits_first;

        unsigned int low_part = data[first_int] >> offset;
        unsigned int high_part = data[first_int + 1] & ((1u << bits_second) - 1u);

        return low_part | (high_part << bits_first);
    }
}

int main(void) {
    int task;
    unsigned int N, i;

    if (scanf("%d", &task) != 1) {
        return 0;
    }

    if (scanf("%u", &N) != 1) {
        return 0;
    }

    if (task == TASK7) {
        unsigned int *v = (unsigned int *)malloc(N * sizeof(unsigned int));
        TDate *dates = (TDate *)malloc(N * sizeof(TDate));

        if (!v || !dates) {
            free(v);
            free(dates);
            return 0;
        }

        for (i = 0; i < N; i++) {
            scanf("%u", &v[i]);
            dates[i] = decode_date_15bits(v[i]);
        }

        qsort(dates, N, sizeof(TDate), cmp_dates);

        for (i = 0; i < N; i++) {
            printf("%u %s %u\n", dates[i].day, luni[dates[i].month - 1], dates[i].year);
        }

        free(v);
        free(dates);
    } else if (task == TASK8) {
        unsigned int data_int_count = (N * BITI_DATA + NR_BITI_UINT - 1) / NR_BITI_UINT;
        unsigned int control_int_count = (data_int_count + NR_BITI_UINT - 1) / NR_BITI_UINT;

        unsigned int *data = (unsigned int *)malloc(data_int_count * sizeof(unsigned int));
        unsigned int *control = (unsigned int *)malloc(control_int_count * sizeof(unsigned int));
        int *ok_int = (int *)malloc(data_int_count * sizeof(int));
        TDate *valid_dates = (TDate *)malloc(N * sizeof(TDate));
        unsigned int valid_count = 0;

        if (!data || !control || !ok_int || !valid_dates) {
            free(data);
            free(control);
            free(ok_int);
            free(valid_dates);
            return 0;
        }

        for (i = 0; i < data_int_count; i++) {
            scanf("%u", &data[i]);
        }

        for (i = 0; i < control_int_count; i++) {
            scanf("%u", &control[i]);
        }

        for (i = 0; i < data_int_count; i++) {
            unsigned int control_bit = (control[i / NR_BITI_UINT] >> (i % NR_BITI_UINT)) & 1u;
            ok_int[i] = (parity_uint(data[i]) == (int)control_bit);
        }

        for (i = 0; i < N; i++) {
            unsigned int start_bit = i * BITI_DATA;
            unsigned int first_int = start_bit / NR_BITI_UINT;
            unsigned int last_int = (start_bit + BITI_DATA - 1) / NR_BITI_UINT;

            if (!ok_int[first_int]) {
                continue;
            }
            if (last_int != first_int && !ok_int[last_int]) {
                continue;
            }

            valid_dates[valid_count++] = decode_date_15bits(extract_15_bits(data, i));
        }

        qsort(valid_dates, valid_count, sizeof(TDate), cmp_dates);

        for (i = 0; i < valid_count; i++) {
            printf("%u %s %u\n",
                   valid_dates[i].day,
                   luni[valid_dates[i].month - 1],
                   valid_dates[i].year);
        }

        free(data);
        free(control);
        free(ok_int);
        free(valid_dates);
    }

    return 0;
}