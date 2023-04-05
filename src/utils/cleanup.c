#include "ft_socket.h"
#include <stdlib.h>

void cleanup(struct Socket *sock) {
    if (sock->target_hostname) {
        free(sock->target_hostname);
    }

    if (sock->target_ip) {
        free(sock->target_ip);
    }

}
