#pragma once

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include "ft_socket.h"
#include "ft_flags.h"

// On macOS, ICMP packets require specific sizes and alignments
#define PACKETSIZE 64        // Standard ping packet size
#define TTL_VAL 64          // Default TTL value

// ICMP packet structure
// On macOS, we need to ensure proper alignment of the icmp header
typedef struct {
    struct icmp icmp_header;  // ICMP header structure (from netinet/ip_icmp.h)
    char data[PACKETSIZE - sizeof(struct icmp)];  // Remaining space for data
} Icmp_Packet;

// Packet statistics and control structure
typedef struct {
    Icmp_Packet icmp;
    int total_packet_sent;
    int packet_received;
    int packet_total;
    int time_to_live;
} Packet;

// Function declarations
void ping(Socket *sock, Options *options, volatile bool *running);
