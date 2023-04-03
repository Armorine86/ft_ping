#include "ft_socket.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

struct Socket *init_socket(struct Socket *socket, char **cmd) {
    socket->target_hostname = strdup(cmd[0]);
    return socket;
}
