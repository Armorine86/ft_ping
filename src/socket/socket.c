#include "ft_socket.h"
#include "ft_errors.h"
#include "ft_packet.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int init_socket(Socket *sock) {
    // Create raw socket - on macOS, this requires root privileges
    sock->fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock->fd < 0) {
        if (getuid() != 0) {
            printf("Permission denied. Try running with sudo\n");
        }
        return -1;
    }

    // Set TTL - macOS requires this to be set explicitly
    int ttl = TTL_VAL;
    if (setsockopt(sock->fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {
        perror("setsockopt TTL");
        return -1;
    }

    // Set receive timeout - important for macOS to prevent blocking
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (setsockopt(sock->fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("setsockopt RCVTIMEO");
        return -1;
    }

    return sock->fd;
}

void socket_setup(Socket *sock) {
    if (init_socket(sock) <= 0) {
        exit_program("Could not initiate Socket", 2);
    }

    printf("Socket Successfully Created!\nSocket FD: %d\n\n", sock->fd);

    // Initialize target address - macOS requires proper address family setup
    memset(&sock->target_addr, 0, sizeof(sock->target_addr));
    sock->target_addr.sin_family = AF_INET;
    sock->target_addr.sin_port = 0;  // ICMP doesn't use ports on macOS
    
    // Convert IP string to network address - macOS uses network byte order
    if (inet_pton(AF_INET, sock->target_ip, &sock->target_addr.sin_addr) <= 0) {
        perror("inet_pton");
        cleanup(sock);
        exit_program("Failed to set target address", 1);
    }

    // Debug output
#ifdef DEBUG
    printf("Socket settings:\n");
    printf("TTL: %d\n", TTL_VAL);
    printf("Target addr family: %d\n", sock->target_addr.sin_family);
    printf("Target addr port: %d\n", ntohs(sock->target_addr.sin_port));
    printf("Target addr IP: %s\n\n", sock->target_ip);
#endif
}
