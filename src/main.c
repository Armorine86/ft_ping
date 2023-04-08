#include "ft_flags.h"
#include "ft_socket.h"
#include "ft_errors.h"
#include "ft_ping.h"
#include "utils.h"
#include <sys/socket.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

bool loop = true;

void sigIntHandler(int dummy) {
    (void)dummy;
    loop = false;
    printf("=== PING REPORT ===\n");
}

int main(int argc, char **argv) {
    if (argc == 1) {
        exit_program(USAGE, 1);
    }
    signal(SIGINT, sigIntHandler);

    Socket sock;
    memset(&sock, 0, sizeof(Socket));

    int flags = 0;

    argv++;
    if (parse_cmd(&flags, &sock, argv, --argc) == false) {
        exit_program(USAGE, 1);
    }

    get_address_info(&sock);
    socket_setup(&sock);
    ping(&sock);
    
    if (flags & VERBOSE) {
        printf("VERBOSE\n");
    } if (flags & SWEEPINCRSIZE) {
        printf("SWEEPINCRSIZE\n");
    }
    
    cleanup(&sock);
    return 0;
}
