#include "ft_ping.h"
#include "ft_flags.h"
#include "utils.h"
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>

int main(int argc, char **argv) {
    struct Ping ping;
    memset(&ping, 0, sizeof(struct Ping));

    ping.socket = (struct Socket *)calloc(1, sizeof(struct Socket));

    argv++;

    if (parse_cmd(&ping, argv, --argc) == false) {
        exit_program(USAGE, 1);
    }

    init_socket(ping.socket, argv);

    if (ping.flags & VERBOSE) {
        printf("VERBOSE\n");
    } if (ping.flags & SWEEPINCRSIZE) {
        printf("SWEEPINCRSIZE\n");
    }
    
    return 0;
}
