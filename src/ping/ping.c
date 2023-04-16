#include "libft.h"
#include "ft_packet.h"
#include "ft_socket.h"
#include "utils.h"
#include "ft_flags.h"
#include "ft_errors.h"
#include <bits/time.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

extern bool loop;

void init_ping(Packet *packet, Icmp_Packet *icmp) {
    memset(packet, 0, sizeof(Packet));
    memset(icmp, 0, sizeof(Icmp_Packet));

    packet->icmp = *icmp;
    packet->time_to_live = TTL_VAL;
}

uint16_t icmp_checksum(void *data, int len) {
    uint16_t *p = data;
    uint32_t checksum = 0;

    while (len > 1) {
        checksum += *p++;
        len -= 2;
    }

    if (len == 1) {
        checksum += *(uint8_t *)p;
    }

    while (checksum >> 16) {
        checksum = (checksum & 0xFFFF) + (checksum >> 16);
    }

    return (uint16_t)checksum;
}

void packet_setup(Socket *sock, Packet *packet, struct iovec *iov, char *msg_buffer) {
    packet->icmp.icmp_header.icmp_type = ICMP_ECHO;
    packet->icmp.icmp_header.icmp_hun.ih_idseq.icd_id = getuid();
    packet->icmp.icmp_header.icmp_hun.ih_idseq.icd_seq = 1;
    packet->icmp.icmp_header.icmp_cksum = icmp_checksum(&packet->icmp, sizeof(packet->icmp));

    iov->iov_base = msg_buffer;
    iov->iov_len = MSG_BUF_SIZE;

    packet->msghdr.msg_name = &sock->target_addr;
    packet->msghdr.msg_namelen = sizeof(sock->target_addr);
    packet->msghdr.msg_iov = iov;
    packet->msghdr.msg_iovlen = 1;

}

void send_ping(Socket *sock, Options *opt, Packet *packet) {
    Timer pckt_timer;
    Timer ping_timer;

    int bytes = 0;
    int result = 0;
    struct iovec iov;

    bool packet_sent = true;

    char msg_buffer[MSG_BUF_SIZE];
    memset(&msg_buffer, 0, sizeof(msg_buffer));

    packet_setup(sock, packet, &iov, msg_buffer);

    clock_gettime(CLOCK_MONOTONIC, &ping_timer.time_start);

    while(loop) {

        clock_gettime(CLOCK_MONOTONIC, &pckt_timer.time_start);

        if ((result = sendto(sock->fd, &packet->icmp, sizeof(packet->icmp), 0,
                            (struct sockaddr *)&sock->target_addr, sizeof(sock->target_addr))) < 0) {
            dprintf(2, "Failed to send packet to target_addr\n");
            packet_sent = false;
        }
        if (opt->flags & FLOOD) {
            write(1, ".", 1);
            continue;
        }

        if (packet_sent) {
            packet->total_packet_sent++;
            if ((bytes = recvmsg(sock->fd, &packet->msghdr, 0)) < 0) {
                dprintf(2, "Failed to receive message from %s\n", sock->hostname);

            } else {

                if (opt->flags & FLOOD) {
                    write(1, "\b", 1);
                    continue;
                }

                (opt->flags & VERBOSE)
                    ? printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.2Lf ms\n",
                        PACKETSIZE,
                        sock->hostname,
                        packet->icmp.icmp_header.icmp_hun.ih_idseq.icd_seq,
                        packet->time_to_live,
                        calculate_time(&pckt_timer))

                    : printf("%d bytes from %s: time=%.2Lf ms\n",
                        PACKETSIZE,
                        sock->hostname,
                        calculate_time(&pckt_timer));

                packet->packet_received++;
            }

            packet->icmp.icmp_header.icmp_hun.ih_idseq.icd_seq++;

            memset(&packet->icmp.icmp_header.icmp_cksum, 0, sizeof(packet->icmp.icmp_header.icmp_cksum));
            packet->icmp.icmp_header.icmp_cksum = icmp_checksum(&packet->icmp, sizeof(packet->icmp));

        }
        sleep((opt->flags & FLOOD) ? 0 : opt->flags & INTERVAL ? opt->interval_sec : 1);
        packet->packet_total++;
    }

    printf("--- %s ping statistics ---\n", sock->web_address);
    printf("%d packets transmitted, %d packets received, %d%% packet loss, time %.2Lfms\n",
            packet->total_packet_sent, packet->packet_received, calc_percentage(packet->total_packet_sent, packet->packet_received, packet->packet_total), calculate_time(&ping_timer));
}

void ping(Socket *sock, Options *options) {

    Packet packet;
    Icmp_Packet icmp;

    init_ping(&packet, &icmp);

    if (setsockopt(sock->fd, IPPROTO_IP, IP_TTL, &packet.time_to_live, sizeof(packet.time_to_live)) != 0) {
        cleanup(sock);
        exit_program("Failed to setup packet TTL", 6);
    } else {
        printf("Socket TTL setup successfull\n");
    }

    send_ping(sock, options, &packet);
    return;
}
