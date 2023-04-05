#include "ft_socket.h"
#include "libft.h"
#include "utils.h"
#include <stdio.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>

void init_socket(struct Socket *sock) {
    sock->fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);

    if (sock->fd < 0) {
        exit_program("Could not initiate Socket", 2);
    } else {
        printf("Socket Successfully Created!\nFD: %d\n", sock->fd);
    }
}

void dns_lookup(struct Socket *sock) {
    struct hostent *host_entity;

    char *ip = (char *)calloc(1, NI_MAXHOST * sizeof(char));

    host_entity = gethostbyname(sock->target_hostname);

    if (host_entity != NULL) {

        // Gets the ip based on the Hostname
        strcpy(ip, inet_ntoa(*(struct in_addr *) host_entity->h_addr));

        (sock->addr_con).sin_family = host_entity->h_addrtype;
        (sock->addr_con).sin_port = htons(0);
        (sock->addr_con).sin_addr.s_addr = *(long*)host_entity->h_addr;

        sock->target_ip = ip;

        printf("DNS successfully resolved an IP for %s\nIP: %s\n", sock->target_hostname, sock->target_ip);
        free(ip);
        free(sock->target_hostname);

    } else {
        exit_program("Couldn't find IP for specified hostname\n", 5);
    }
}

void reverse_dns_lookup(struct Socket *sock) {

    struct sockaddr_in temp_addr;
    socklen_t len;
    char hostname[NI_MAXHOST];
    memset(&hostname, 0, sizeof(hostname));

    temp_addr.sin_family = AF_INET;
    temp_addr.sin_addr.s_addr = inet_addr(sock->target_ip);
    len = sizeof(struct sockaddr_in);


    getnameinfo((struct sockaddr *) &temp_addr, len, hostname, sizeof(hostname), NULL, 0, NI_NAMEREQD);

    if (strlen(hostname) <= 0) {
        exit_program("Could not resolve reverse lookup of hostname\n", 4);
    } else {
        if (sock->target_hostname) {
            free(sock->target_hostname);
        }
        sock->target_hostname = (char *)calloc(strlen(hostname) + 1, sizeof(char));
        ft_strcpy(sock->target_hostname, hostname);
        free(sock->target_ip);
        printf("Successfully Resolved Hostname for given IP: %s\nHostname: %s\n", sock->target_ip ,sock->target_hostname);
    }
}

void socket_setup(struct Socket *sock) {
    sock->addrlen = sizeof(sock->addr_con);

    init_socket(sock);
    if (sock->target_hostname) {
        dns_lookup(sock);
    } else if (sock->target_ip) {
        reverse_dns_lookup(sock);
    }
}
