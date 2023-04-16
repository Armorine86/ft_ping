#include "ft_flags.h"
#include "utils.h"
#include "libft.h"
#include <string.h>
#include <stdio.h>

void set_flags(Options *opt, char **argv, char c) {
    switch (c) {
        case 'v':
            opt->flags |= VERBOSE;
            break;
        
        case 'h':
            opt->flags |= HELP;
            break;
        
        case 'f':
            opt->flags |= FLOOD;
            break;
        case 'i':
            opt->flags |= INTERVAL;
            ++argv;
            opt->interval_sec = ft_atoi(*argv);
            break;
    }
}

bool validate_flag(char c) {
    return ft_strchr(POSSIBLE_FLAGS, c);
}

bool validate_flags(Options *options, char **argv, char *str) {
    for (int i = 0; str[i]; ++i) {
        if (!ft_strchr(POSSIBLE_FLAGS, str[i])) {
            return false;
        }
        set_flags(options, argv, str[i]);
    }
    return true;
}

bool parse_cmd(Options *options, Socket *sock, char **argv, int argc) {
    for (; argc > 1; ++argv, --argc) {
        ft_print_tab(argv);

        for (int i = 0; *argv[i]; i++) {
            if ((*argv)[i] == '-' && strlen(*argv) == 2) {
                if (!validate_flag((*argv)[1])) {
                    return false;
                }
                // separated flags i.e: -v -h -i -f
                set_flags(options, argv, (*argv)[1]);
                break;

            } else if (*argv[i] == '-' && strlen(*argv) > 2) {
                // multiple flags  i.e: -vhif
                validate_flags(options, argv, ++(*argv));
                break;
            }
            break;
        }
    }

    char **hostname = ft_split(*argv, '.');
    size_t size = ft_tabsize(hostname);
    ft_freetab(hostname);
    
    if (size < 2) {
        return false;

    } else if (size == 2 || size == 3) {
        sock->web_address = ft_strdup(*argv);
    } else if (size == 4) {
        sock->target_ip = ft_strdup(*argv);
    }
    return true;
}
