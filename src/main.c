#include "ft_flags.h"
#include "ft_socket.h"
#include "ft_errors.h"
#include "ft_packet.h"
#include "utils.h"
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>

// Define global variables
volatile bool g_running = true;
int g_packets_transmitted = 0;
int g_packets_received = 0;
struct timespec g_start_time;

static void signal_handler(int signo) {
    if (signo == SIGINT) {
        printf("\n");  // Move to a new line after ^C
        g_running = false;
        print_stats();
        exit(0);
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("%s", USAGE);
        return 1;
    }

    // Set up signal handling
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return 1;
    }

    Socket sock = {0};
    Options options = {0};

    if (!parse_cmd(&options, &sock, ++argv, --argc)) {
        printf("%s", USAGE);
        return 1;
    }

    if (options.flags & HELP) {
        printf("%s", USAGE);
        return 0;
    }

    // Record start time
    clock_gettime(CLOCK_MONOTONIC, &g_start_time);

    get_address_info(&sock);
    socket_setup(&sock);
    
    // Pass the running flag to ping function
    ping(&sock, &options, &g_running);
    
    cleanup(&sock);
    return 0;
}
