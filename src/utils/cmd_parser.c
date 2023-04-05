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
    return ft_strchr(POSSIBLE_FLAGS, c);
}

bool validate_flags(int *flags, char *str) {
    for (int i = 0; str[i]; ++i) {
        if (!ft_strchr(POSSIBLE_FLAGS, str[i])) {
            return false;
        }
        set_flags(flags, str[i]);
    }
    return true;
}

bool parse_cmd(int *flags, struct Socket *sock, char **argv, int argc) {
    for (; argv; argv++, argc--) {
        if ((argc == 1 || argc == 2) && (*argv)[0] != '-') {
            break;
        }
        for (int i = 0; *argv[i]; i++) {

            if ((*argv)[i] == '-' && strlen(*argv) == 2) {
                if (!validate_flag((*argv)[1])) {
                    return false;
                }

                set_flags(flags, (*argv)[1]);
                break;

            } else if (*argv[i] == '-' && strlen(*argv) > 2) {
                validate_flags(flags, *++argv);
                break;
            }
            break;
        }
    }

    char **hostname = ft_split(*argv, '.');
    size_t size = ft_tabsize(hostname);

    ft_freetab(hostname);
    if (size == 2 || size == 3) {
        sock->target_hostname = *argv;
    } else if (size == 4) {
        sock->target_ip = *argv;
    }
    return true;
}
