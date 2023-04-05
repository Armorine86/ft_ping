#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>

struct Socket {
    int fd;
    int addrlen;
    char *target_ip;
    char *target_hostname;
    struct sockaddr_in addr_con;
};

void init_socket(struct Socket *sock);
void socket_setup(struct Socket *sock);
