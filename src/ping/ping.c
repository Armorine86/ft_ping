#include "libft.h"
#include "ft_ping.h"
#include "ft_socket.h"
#include "utils.h"
#include "ft_errors.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/_types/_iovec_t.h>
#include <sys/socket.h>
#include <time.h>

extern bool loop;

void init_ping(Packet *packet, Icmp_Packet *icmp, Timer *timer) {
    memset(packet, 0, sizeof(Packet));
    memset(icmp, 0, sizeof(Icmp_Packet));

    packet->icmp = *icmp;
    timer->tv_out.tv_sec = RECEIVE_TIMEOUT;
    timer->tv_out.tv_usec = 0;
    packet->time_to_live = TTL_VAL;
    clock_gettime(CLOCK_MONOTONIC, &timer->total_time_start);
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
    packet->icmp.icmp_header.icmp_hun.ih_idseq.icd_seq = 0;
    packet->icmp.icmp_header.icmp_cksum = icmp_checksum(&packet->icmp, sizeof(&packet->icmp));

    iov->iov_base = msg_buffer;
    iov->iov_len = MSG_BUF_SIZE;

    packet->msghdr.msg_name = &sock->target_addr;
    packet->msghdr.msg_namelen = sizeof(sock->target_addr);
    packet->msghdr.msg_iov = iov;
    packet->msghdr.msg_iovlen = 1;

}

void send_ping(Socket *sock, Packet *packet, Timer *timer) {
    int bytes = 0;
    struct iovec iov;
    int result = 0;

    bool packet_sent = true;

    char msg_buffer[MSG_BUF_SIZE];
    memset(&msg_buffer, 0, sizeof(msg_buffer));

    packet_setup(sock, packet, &iov, msg_buffer);

    while(loop) {

        clock_gettime(CLOCK_MONOTONIC, &timer->time_start);

        if ((result = sendto(sock->fd, &packet->icmp, sizeof(packet->icmp), 0,
                            (struct sockaddr *)&sock->target_addr, sizeof(sock->target_addr))) < 0) {
            dprintf(2, "Failed to send packet to target_addr\n");
            packet_sent = false;
        }

        if (packet_sent) {
            if ((bytes = recvmsg(sock->fd, &packet->msghdr, 0)) < 0) {
                dprintf(2, "Failed to receive message from target_addr\n");

            } else {
                clock_gettime(CLOCK_MONOTONIC, &timer->time_end);
                packet->ping_return_count++;

                timer->time_elapsed = ((double)timer->time_end.tv_nsec - timer->time_start.tv_nsec) / 1000000.0;
                timer->pkt_round_msec = (timer->time_end.tv_sec - timer->time_start.tv_sec) * 1000.0 + timer->time_elapsed;
                printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%Lf ms\n",
                    PACKETSIZE,
                    sock->target_ip,
                    packet->icmp.icmp_header.icmp_hun.ih_idseq.icd_seq,
                    packet->time_to_live,
                    timer->pkt_round_msec);
            }
        }
    }
}

void ping(Socket *sock) {

    Packet packet;
    Icmp_Packet icmp;
    Timer timer;

    init_ping(&packet, &icmp, &timer);

    if (setsockopt(sock->fd, IPPROTO_IP, IP_TTL, &packet.time_to_live, sizeof(packet.time_to_live)) != 0) {
        cleanup(sock);
        exit_program("Failed to setup packet TTL", 6);
    } else {
        printf("Socket TTL setup successfull\n");
    }

    send_ping(sock, &packet, &timer);
    return;
}
