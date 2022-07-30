#include <stdio.h>

#define ASSERT(condition) \
    if(!(condition)) { \
        fprintf(stderr, "assert failed at %s:%d\n", __FILE__, __LINE__);\
        return 1;\
    }
