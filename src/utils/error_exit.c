#include <stdio.h>
#include <stdlib.h>

void exit_program(char *msg, int exit_code) {
    dprintf(2, "%s\n", msg);
    exit(exit_code);
}
