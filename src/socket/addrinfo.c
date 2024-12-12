#include "ft_errors.h"
#include "ft_socket.h"
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>

static void get_target_ip(Socket *sock) {
    struct addrinfo hints, *result = NULL;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_CANONNAME;

    int ret = getaddrinfo(sock->web_address, NULL, &hints, &result);
    if (ret != 0) {
        printf("getaddrinfo error: %s\n", gai_strerror(ret));
        exit_program("Could not resolve address info\n", 2);
    }

    sock->target_addr = *(struct sockaddr_in *)result->ai_addr;
    sock->target_addr.sin_family = AF_INET;
    sock->target_addr.sin_port = 0;  // ICMP doesn't use ports

    char ip_address[INET_ADDRSTRLEN];
    if (!inet_ntop(AF_INET, &sock->target_addr.sin_addr, ip_address, INET_ADDRSTRLEN)) {
        freeaddrinfo(result);
        exit_program("Could not retrieve target's ip address", 3);
    }

    sock->target_ip = strdup(ip_address);
    printf("Successfully retrieved target's IP address\nIP: %s\n\n", sock->target_ip);

    freeaddrinfo(result);
}

static void get_target_hostname(Socket *sock) {
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    if (strcmp(sock->target_ip, "127.0.0.1") == 0)
        return;

    if (getnameinfo((struct sockaddr *)&sock->target_addr, 
                    sizeof(sock->target_addr), 
                    host, sizeof(host), 
                    service, sizeof(service), 
                    NI_NAMEREQD) < 0) {
        exit_program("Could not resolve target's host name", 4);
    }

    sock->hostname = strdup(host);
    printf("Successfully retrieved target's hostname\nHostname: %s\n\n", sock->hostname);
}

void get_address_info(Socket *sock) {
    if (sock->web_address) {
        get_target_ip(sock);
    }
    if (sock->target_ip) {
        get_target_hostname(sock);
    }
}
