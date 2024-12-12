#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern int g_packets_transmitted;
extern int g_packets_received;
extern struct timespec g_start_time;

// Calculate packet loss percentage
int calc_percentage(int sent, int received, int total) {
    if (total == 0) return 0;
    return ((sent - received) * 100) / total;
}

// Calculate elapsed time in milliseconds - uses macOS monotonic clock
long double calculate_time(Timer *timer) {
    clock_gettime(CLOCK_MONOTONIC, &timer->time_end);
    
    // Convert to milliseconds with nanosecond precision
    timer->time_elapsed = ((timer->time_end.tv_sec - timer->time_start.tv_sec) * SEC_TO_MS) +
                         ((timer->time_end.tv_nsec - timer->time_start.tv_nsec) / NANO_TO_MS);
    
    return timer->time_elapsed;
}

// Calculate ICMP checksum - required for macOS packet validation
uint16_t icmp_checksum(void *data, size_t len) {
    uint16_t *buf = data;
    uint32_t sum = 0;
    
    // Standard checksum calculation
    while (len > 1) {
        sum += *buf++;
        len -= 2;
    }
    
    // Handle odd byte if present
    if (len == 1) {
        sum += *(uint8_t *)buf;
    }
    
    // Add carries and return one's complement
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    
    return ~sum;
}

void print_stats(void) {
    struct timespec end_time;
    clock_gettime(CLOCK_MONOTONIC, &end_time);
    
    double time_ms = (end_time.tv_sec - g_start_time.tv_sec) * SEC_TO_MS +
                    (end_time.tv_nsec - g_start_time.tv_nsec) / NANO_TO_MS;
    
    printf("\n--- %s ping statistics ---\n", "ping");
    printf("%d packets transmitted, %d packets received, %.1f%% packet loss\n",
           g_packets_transmitted,
           g_packets_received,
           100.0 * (g_packets_transmitted - g_packets_received) / g_packets_transmitted);
    printf("round-trip min/avg/max = %.3f/%.3f/%.3f ms\n",
           0.0, // Replace with actual min
           time_ms / g_packets_received,
           0.0); // Replace with actual max
}
