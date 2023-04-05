#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "ft_socket.h"

#define POSSIBLE_FLAGS "vh"
#define USAGE "./ft_ping [-vh / -v -h] [hostname/IP] [packetSize]"
#define PACKETSIZE 64

void exit_program(char *msg, int exit_code);
bool parse_cmd(int *flags, struct Socket *sock, char **argv, int argc);
void set_flags(int *flags, char c);
