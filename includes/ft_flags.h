#pragma once

#define POSSIBLE_FLAGS "vhfi"

typedef enum Flags {
    VERBOSE = 1,
    HELP = 2,
    FLOOD = 4,
    INTERVAL = 8,
} Flags;

typedef struct Options {
    int flags;
    int interval_sec;
} Options;
