#include "utils.h"

size_t tab_size(char **tab) {
    size_t i = 0;    
    for (; tab[i]; i++) {}
    return i;
}
