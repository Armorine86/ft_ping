#include "ft_ping.h"
#include "ft_flags.h"
#include "utils.h"
#include "libft.h"
#include <string.h>
#include <stdio.h>

void set_flags(int *flags, char c) {
    switch (c) {
        case 'v':
            *flags |= VERBOSE;
            break;
        
        case 'h':
            *flags |= SWEEPINCRSIZE;
            break;
    }
}

bool validate_flag(char c) {
    return strchr(POSSIBLE_FLAGS, c);
}

bool validate_flags(int *flags, char *str) {
    for (int i = 0; str[i]; ++i) {
        if (!strchr(POSSIBLE_FLAGS, str[i])) {
            return false;
        }
        set_flags(flags, str[i]);
    }
    return true;
}

bool parse_cmd(struct Ping *ping, char **argv, int argc) {
    for (; argv; argv++, argc--) {
        if ((argc == 1 || argc == 2) && (*argv)[0] != '-') {
            break;
        }
        for (int i = 0; *argv[i]; i++) {

            if ((*argv)[i] == '-' && strlen(*argv) == 2) {
                if (!validate_flag((*argv)[1])) {
                    return false;
                }

                set_flags(&ping->flags, (*argv)[1]);
                break;

            } else if (*argv[i] == '-' && strlen(*argv) > 2) {
                validate_flags(&ping->flags, *++argv);
                break;
            }
            break;
        }
    }

    char **hostname = ft_split(*argv, '.');
    size_t size = tab_size(hostname);

    if (size == 2 || size == 3) {
        ping->socket->target_hostname = ft_strdup(*argv);
    } else if (size == 4) {
        ping->socket->target_ip = ft_strdup(*argv);
    }
    return true;
}
