#pragma once

#include <sys/socket.h>

struct Socket {
    int fd;
    int addrlen;
    char *target_ip;
    char *target_hostname;
    struct sockaddr_in *addr_con;
};

struct Socket *init_socket(struct Socket *socket, char **cmd);
