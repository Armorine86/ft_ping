#include "ft_socket.h"
#include <stdlib.h>
#include <unistd.h>

void cleanup(Socket *sock) {
    if (sock->fd > 0) {
        close(sock->fd);
    }
    free(sock->web_address);
    free(sock->target_ip);
    free(sock->hostname);
}
