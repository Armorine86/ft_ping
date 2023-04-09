#pragma once

#include "ft_socket.h"
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>

// Standard size of an ICMP packet is 64 bytes
#define PACKETSIZE 64
#define RECEIVE_TIMEOUT 2
#define MSG_BUF_SIZE 2048

/* Number of router hops before the packet expires */
#define TTL_VAL 64

typedef struct Icmp_Packet {
    struct icmp icmp_header;
    char msg[PACKETSIZE - sizeof(struct icmp)];
} Icmp_Packet;

typedef struct Packet {
    int time_to_live;
    int addr_len;
    int packet_sent;
    int packet_received;
    int packet_total;
    struct msghdr msghdr;
    struct sockaddr_in return_address;
    Icmp_Packet icmp;
} Packet;

void ping(struct Socket *sock);
