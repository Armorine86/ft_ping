#pragma once

#define USAGE "\
Usage: ping [-vhfq] [-c count] [-i interval] [-s packetsize] [-t ttl] destination\n\
Options:\n\
  -v            verbose output\n\
  -h            show this help message\n\
  -f            flood ping (root only)\n\
  -q            quiet output\n\
  -c count      stop after sending 'count' packets\n\
  -i interval   wait 'interval' seconds between sending each packet\n\
  -s size       specify the number of data bytes to be sent (default: 56)\n\
  -t ttl        specify the IP Time To Live (default: 64)\n\
destination     dns name or ip address\n"

typedef enum Errors {
    TTL_EXPIRED,
    HOST_UNREACHABLE,
    TIMEOUT,
    UNKNOWN_HOST
} Errors;

void exit_program(const char *msg, int exit_code);
