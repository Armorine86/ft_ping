#pragma once

#include "ft_socket.h"
#include "ft_flags.h"

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

typedef struct Timer {
    struct timespec time_start;
    struct timespec time_end;
    double time_elapsed;
    long double pkt_round_msec;
} Timer;

bool parse_cmd(Options *options, struct Socket *sock, char **argv, int argc);
// void set_flags(Options *options, char *str, char c);
void cleanup(struct Socket *sock);
int calc_percentage(int sent, int received, int total);
long double calculate_time(Timer *timer);
