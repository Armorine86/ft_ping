#pragma once

#include "ft_socket.h"

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

typedef struct Timer {
    struct timespec time_start;
    struct timespec time_end;
    struct timespec total_time_start;
    struct timespec total_time_end;
    struct timeval tv_out;
    double time_elapsed;
    long double pkt_round_msec;
    long double total_msec;
} Timer;

bool parse_cmd(int *flags, struct Socket *sock, char **argv, int argc);
void set_flags(int *flags, char c);
void cleanup(struct Socket *sock);
int calc_percentage(int sent, int received, int total);
