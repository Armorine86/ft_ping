#include "ft_flags.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Default values for options
#define DEFAULT_PACKET_SIZE 56
#define DEFAULT_TTL 64
#define DEFAULT_COUNT 0  // 0 means infinite
#define DEFAULT_INTERVAL 1

static void set_flags(Options *opt, char **argv, char c, int *skip) {
    switch (c) {
        case 'v':
            opt->flags |= VERBOSE;
            break;
        case 'h':
            opt->flags |= HELP;
            break;
        case 'f':
            if (getuid() == 0) {  // Only root can use flood
                opt->flags |= FLOOD;
            } else {
                printf("flood ping requires root privileges\n");
                exit(1);
            }
            break;
        case 'q':
            opt->flags |= QUIET;
            break;
        case 'i':
            if (argv[1] && argv[1][0] != '-') {
                opt->flags |= INTERVAL;
                opt->interval_sec = atoi(argv[1]);
                if (opt->interval_sec <= 0) {
                    printf("invalid interval: %s\n", argv[1]);
                    exit(1);
                }
                *skip = 1;
            }
            break;
        case 'c':
            if (argv[1] && argv[1][0] != '-') {
                opt->flags |= COUNT;
                opt->count = atoi(argv[1]);
                if (opt->count <= 0) {
                    printf("invalid count: %s\n", argv[1]);
                    exit(1);
                }
                *skip = 1;
            }
            break;
        case 's':
            if (argv[1] && argv[1][0] != '-') {
                opt->flags |= PACKETSIZE;
                opt->packet_size = atoi(argv[1]);
                if (opt->packet_size < 0 || opt->packet_size > 65507) {
                    printf("invalid packet size: %s\n", argv[1]);
                    exit(1);
                }
                *skip = 1;
            }
            break;
        case 't':
            if (argv[1] && argv[1][0] != '-') {
                opt->flags |= TTL;
                opt->ttl = atoi(argv[1]);
                if (opt->ttl <= 0 || opt->ttl > 255) {
                    printf("invalid ttl: %s\n", argv[1]);
                    exit(1);
                }
                *skip = 1;
            }
            break;
    }
}

static bool validate_flag(char c) {
    return strchr(POSSIBLE_FLAGS, c) != NULL;
}

bool parse_cmd(Options *options, Socket *sock, char **argv, int argc) {
    int skip = 0;

    // Initialize options with default values
    options->flags = 0;
    options->interval_sec = DEFAULT_INTERVAL;
    options->count = DEFAULT_COUNT;
    options->packet_size = DEFAULT_PACKET_SIZE;
    options->ttl = DEFAULT_TTL;

    while (argc > 0) {
        if (skip) {
            skip = 0;
            argv++;
            argc--;
            continue;
        }

        if (argv[0][0] == '-') {
            char *flags = argv[0] + 1;
            for (size_t i = 0; flags[i]; i++) {
                if (!validate_flag(flags[i])) {
                    printf("invalid option -- '%c'\n", flags[i]);
                    return false;
                }
                set_flags(options, argv, flags[i], &skip);
            }
        } else {
            sock->web_address = strdup(argv[0]);
            return true;
        }

        argv++;
        argc--;
    }

    return sock->web_address != NULL;
}
