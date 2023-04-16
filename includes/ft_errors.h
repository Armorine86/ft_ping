#pragma once

#define USAGE "Usage\n \
    ping [options] <destination>\n\
Options:\n\
    <destination>       dns name or ip address\n\
    -v                  verbose output\n"

typedef enum Errors {
    TTL_EXPIRED,
    HOST_UNREACHABLE,
    TIMEOUT,
    UNKNOWN_HOST
} Errors;

void exit_program(char *msg, int exit_code);
