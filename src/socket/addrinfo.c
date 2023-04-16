#include "ft_errors.h"
#include "ft_socket.h"
#include "libft.h"
#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void get_target_ip(Socket *sock) {
    struct addrinfo hints, *result = NULL, *head = NULL;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;

    if (getaddrinfo(sock->web_address, NULL, &hints, &result)) {
        exit_program("Could not resolve address info\n", 2);
    }

    head = result;
    sock->target_addr = *(struct sockaddr_in *)head->ai_addr;

    char ip_address[1024];
    memset(&ip_address, 0, sizeof(ip_address));
    
    const char *res = inet_ntop(AF_INET, &sock->target_addr.sin_addr, ip_address, 1024);
    if (!res) {
        exit_program("Could not retrieve target's ip address", 3);
    }

    sock->target_ip = ft_strdup(ip_address);
    printf("Successfully retrieved target's IP address\nIP: %s\n\n", sock->target_ip);

    freeaddrinfo(result);
}

void get_target_hostname(Socket *sock) {
    char host[1024];
    char service[32];

    if (ft_strcmp(sock->target_ip, "127.0.0.1") == 0)
        return;
    if (getnameinfo((struct sockaddr *)&sock->target_addr, sizeof(sock->target_addr), host, sizeof(host), service, sizeof(service), NI_NOFQDN) < 0) {
        exit_program("Could not resolve target's host name", 4);
    }

    if (strlen(host) > 0) {
        sock->hostname = ft_strdup(host);
        printf("Successfully retrieved target's hostname\nHostname: %s\n\n", sock->hostname);
    }
}

void get_address_info(Socket *sock) {
    if (sock->web_address) {
        get_target_ip(sock);
    }

    if (sock->target_ip) {
        get_target_hostname(sock);
    }
}
