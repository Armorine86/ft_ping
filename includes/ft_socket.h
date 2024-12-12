#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <stdbool.h>

// Socket structure for managing network connections
// On macOS, we need to handle both IPv4 and hostname resolution
typedef struct {
    int fd;                     // Socket file descriptor
    char *target_ip;           // Target IP address string
    char *web_address;         // Original input address (hostname or IP)
    char *hostname;            // Resolved hostname
    struct sockaddr_in target_addr;  // Socket address structure for IPv4
} Socket;

// Function declarations for socket operations
void socket_setup(Socket *sock);      // Initialize socket with macOS-specific options
void get_address_info(Socket *sock);  // Resolve addresses using macOS DNS
void cleanup(Socket *sock);           // Cleanup resources
