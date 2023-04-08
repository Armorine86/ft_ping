#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>

typedef struct Socket {
    int fd;
    char *target_ip;
    char *web_address;
    char *hostname;
    struct sockaddr_in target_addr;
} Socket;

int init_socket(struct Socket *sock);
void socket_setup(struct Socket *sock);
void get_address_info(Socket *sock);
