#pragma once

// Possible flags for ping command - matches macOS ping options
#define POSSIBLE_FLAGS "vhficstq"

// Flag bits for different options
typedef enum {
    VERBOSE = 1,     // Show detailed output including TTL
    HELP = 2,        // Show help message
    FLOOD = 4,       // Flood ping (rapid pings)
    INTERVAL = 8,    // Custom interval between pings
    COUNT = 16,      // Stop after sending count packets
    PACKETSIZE = 32, // Set packet size
    TTL = 64,        // Set TTL value
    QUIET = 128      // Quiet output
} Flags;

// Options structure to hold command line settings
typedef struct {
    int flags;          // Bitwise combination of active flags
    int interval_sec;   // Interval between pings (when -i is used)
    int count;          // Number of packets to send (when -c is used)
    int packet_size;    // Custom packet size (when -s is used)
    int ttl;           // Custom TTL value (when -t is used)
} Options;
