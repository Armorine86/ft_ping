#pragma once

#include "ft_ping.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define POSSIBLE_FLAGS "vh"

void exit_program(char *msg, int exit_code);
bool parse_cmd(struct Ping *ping, char **argv, int argc);
void set_flags(int *flags, char c);
size_t tab_size(char **tab);
