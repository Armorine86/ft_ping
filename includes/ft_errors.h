#pragma once

#define USAGE "./ft_ping [-vh / -v -h] [hostname/IP] [packetSize]"

typedef enum Errors {
    TTL_EXPIRED,
    HOST_UNREACHABLE,
    TIMEOUT,
    UNKNOWN_HOST
} Errors;

void exit_program(char *msg, int exit_code);
