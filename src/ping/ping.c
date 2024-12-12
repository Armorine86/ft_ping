#include "ft_packet.h"
#include "ft_socket.h"
#include "ft_flags.h"
#include "utils.h"
#include "ft_errors.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

// Declare external globals that are defined in main.c
extern int g_packets_transmitted;
extern int g_packets_received;
extern struct timespec g_start_time;

// Initialize ICMP packet - macOS requires specific header formatting
static void packet_setup(Packet *packet) {
    // Clear memory first - important for consistent behavior on macOS
    memset(&packet->icmp, 0, sizeof(Icmp_Packet));
    
    // Set up the ICMP header - macOS uses network byte order
    packet->icmp.icmp_header.icmp_type = ICMP_ECHO;    // Echo request type
    packet->icmp.icmp_header.icmp_code = 0;            // Echo request code
    packet->icmp.icmp_header.icmp_id = getpid() & 0xFFFF;  // Use PID as ID
    packet->icmp.icmp_header.icmp_seq = 1;             // Initial sequence number
    
    // Fill data section - helps with packet validation on macOS
    for (size_t i = 0; i < (PACKETSIZE - sizeof(struct icmp)); i++) {
        packet->icmp.data[i] = (unsigned char)i;
    }

    // Calculate checksum - critical for macOS packet validation
    packet->icmp.icmp_header.icmp_cksum = 0;  // Must be 0 before calculation
    packet->icmp.icmp_header.icmp_cksum = icmp_checksum(&packet->icmp.icmp_header, sizeof(struct icmp));
}

void ping(Socket *sock, Options *options, volatile bool *running) {
    Packet packet = {0};
    Timer pckt_timer = {0};
    Timer ping_timer = {0};
    struct msghdr msg = {0};
    struct iovec iov = {0};
    char msg_buffer[PACKETSIZE];
    int bytes = 0;
    int result = 0;

    packet_setup(&packet);

    // Setup message structure for receiving - macOS specific alignment
    memset(&msg, 0, sizeof(msg));
    memset(&iov, 0, sizeof(iov));
    memset(msg_buffer, 0, sizeof(msg_buffer));

    iov.iov_base = msg_buffer;
    iov.iov_len = sizeof(msg_buffer);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_name = &sock->target_addr;
    msg.msg_namelen = sizeof(sock->target_addr);

    clock_gettime(CLOCK_MONOTONIC, &ping_timer.time_start);

    while(*running) {
        // Check if we've reached the count limit
        if ((options->flags & COUNT) && g_packets_transmitted >= options->count) {
            *running = false;
            print_stats();
            break;
        }

        clock_gettime(CLOCK_MONOTONIC, &pckt_timer.time_start);

        result = sendto(sock->fd, 
                       &packet.icmp.icmp_header,
                       sizeof(struct icmp),
                       0,
                       (struct sockaddr *)&sock->target_addr,
                       sizeof(struct sockaddr_in));

        if (result < 0) {
            printf("sendto error: %s (errno: %d)\n", strerror(errno), errno);
            cleanup(sock);
            exit_program("Failed to send packet", 1);
        }

        g_packets_transmitted++;  // Increment global counter

        if (options->flags & FLOOD) {
            write(1, ".", 1);
        }

        bytes = recvmsg(sock->fd, &msg, 0);
        if (bytes < 0) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                perror("recvmsg failed");
                continue;
            }
        } else {
            struct ip *ip_header = (struct ip *)msg_buffer;
            int ip_header_len = ip_header->ip_hl << 2;
            struct icmp *icmp_reply = (struct icmp *)(msg_buffer + ip_header_len);

            if (icmp_reply->icmp_type == ICMP_ECHOREPLY &&
                icmp_reply->icmp_id == (getpid() & 0xFFFF)) {
                g_packets_received++;  // Increment global counter
                
                if (!(options->flags & QUIET)) {  // Only print if not quiet
                    if (options->flags & VERBOSE) {
                        printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.2Lf ms\n",
                            bytes - ip_header_len,
                            sock->hostname,
                            icmp_reply->icmp_seq,
                            ip_header->ip_ttl,
                            calculate_time(&pckt_timer));
                    } else {
                        printf("%d bytes from %s: time=%.2Lf ms\n",
                            bytes - ip_header_len,
                            sock->hostname,
                            calculate_time(&pckt_timer));
                    }
                }

                if (options->flags & FLOOD) {
                    write(1, "\b \b", 3);
                }
            }
        }

        // Prepare next packet
        packet.icmp.icmp_header.icmp_seq++;
        packet.icmp.icmp_header.icmp_cksum = 0;
        packet.icmp.icmp_header.icmp_cksum = icmp_checksum(&packet.icmp.icmp_header, sizeof(struct icmp));

        // Handle delay between pings
        if (!(options->flags & FLOOD)) {
            sleep(options->flags & INTERVAL ? options->interval_sec : 1);
        }
    }
}
