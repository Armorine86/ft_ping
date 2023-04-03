#pragma once

#include <ft_socket.h>

#define USAGE "./ft_ping [-vh / -v -h] [hostname/IP] [packetSize]"

struct Ping {
    struct Socket *socket;
    int flags;
};
