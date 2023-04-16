#include "utils.h"

int calc_percentage(int sent, int received, int total) {
    int res = 0;

    res = sent - received;

    return (res / total) * 100;
}

long double calculate_time(Timer *timer) {
    clock_gettime(CLOCK_MONOTONIC, &timer->time_end);
    timer->time_elapsed = ((double)timer->time_end.tv_nsec - timer->time_start.tv_nsec) / 1000000.0;

    return (((timer->time_end.tv_sec - timer->time_start.tv_sec) * 1000.0) + timer->time_elapsed);
}
