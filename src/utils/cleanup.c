#include "ft_socket.h"
#include <netdb.h>
#include <stdlib.h>

void cleanup(Socket *sock) {
    if (sock->web_address) {
        free(sock->web_address);
    }

    if (sock->target_ip) {
        free(sock->target_ip);
    }

    if (sock->hostname){
        free(sock->hostname);
    }
}
