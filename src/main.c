#include "ft_flags.h"
#include "ft_socket.h"
#include "utils.h"
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc < 1) {
        exit_program(USAGE, 1);
    }

    struct Socket sock;
    memset(&sock, 0, sizeof(struct Socket));

    int flags = 0;

    argv++;
    if (parse_cmd(&flags, &sock, argv, --argc) == false) {
        exit_program(USAGE, 1);
    }

    socket_setup(&sock);

    if (flags & VERBOSE) {
        printf("VERBOSE\n");
    } if (flags & SWEEPINCRSIZE) {
        printf("SWEEPINCRSIZE\n");
    }
    
    return 0;
}
