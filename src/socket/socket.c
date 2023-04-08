#include "ft_socket.h"
#include "ft_dns.h"
#include "ft_errors.h"
#include "libft.h"
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int init_socket(Socket *sock) {
    if (getuid()) {
        return ((sock->fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP)));
    }
    return ((sock->fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)));
}

void socket_setup(Socket *sock) {
    if (init_socket(sock)) {
        printf("Socket Successfully Created!\nSocker FD: %d\n\n", sock->fd);
    } else {
        exit_program("Could not initiate Socket", 2);
    }
}
