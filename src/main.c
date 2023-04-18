#include "ft_flags.h"
#include "ft_socket.h"
#include "ft_errors.h"
#include "ft_packet.h"
#include "utils.h"
#include "libft.h"
#include <sys/socket.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

bool loop = true;

void sigIntHandler(int dummy) {
    (void)dummy;
    loop = false;
}

int main(int argc, char **argv) {
    if (argc == 1) {
        exit_program(USAGE, 1);
    }

    signal(SIGQUIT, sigIntHandler);
    signal(SIGINT, sigIntHandler);

    Socket sock;
    memset(&sock, 0, sizeof(Socket));

    Options options;
    memset(&options, 0, sizeof(Options));

    if (parse_cmd(&options, &sock, ++argv, --argc) == false) {
        exit_program(USAGE, 1);
    }

    if (options.flags & HELP) {
        exit_program(USAGE, 0);
    }

    get_address_info(&sock);
    socket_setup(&sock);
    ping(&sock, &options);
    
    cleanup(&sock);
    return 0;
}
