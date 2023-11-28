#include "save_stdlib.h"
#include <stdio.h>
#include <stdlib.h>


void* saveMalloc(size_t size) {
    void* p = malloc(size);
    if (p == NULL) {
        printf("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    return p;
}
