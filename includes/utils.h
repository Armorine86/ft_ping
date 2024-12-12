#pragma once

#include "ft_socket.h"
#include "ft_flags.h"
#include <time.h>

#define SEC_TO_MS 1000.0
#define NANO_TO_MS 1000000.0

// Timer structure for precise timing measurements
typedef struct {
    struct timespec time_start;
    struct timespec time_end;
    double time_elapsed;
} Timer;

// Function declarations
bool parse_cmd(Options *options, Socket *sock, char **argv, int argc);
int calc_percentage(int sent, int received, int total);
long double calculate_time(Timer *timer);
uint16_t icmp_checksum(void *data, size_t len);
void print_stats(void);
