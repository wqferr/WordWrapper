#include "misc/utils.h"

#include <stdlib.h>
#include <string.h>

void swap(void *a, void *b, size_t bytes) {
    void *tmp = (void *) malloc(bytes);
    memcpy(tmp, a, bytes);
    memcpy(a, b, bytes);
    memcpy(b, tmp, bytes);
    free(tmp);
}
