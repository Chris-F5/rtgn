#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "./utils.h"

void* emalloc(size_t size)
{
    void* p;
    p = malloc(size);
    if(p == NULL)
        die("failed to allocate memory");
    return p;
}

void* erealloc(void* ptr, size_t size)
{
    void* p;
    p = realloc(ptr, size);
    if(p == NULL)
        die("failed to reallocate memory");
    return p;
}

void die(const char* format, ...)
{
    va_list vargs;
    va_start(vargs, format);
    fprintf(stderr, "[RTGN ERROR] ");
    vfprintf(stderr, format, vargs);
    fprintf(stderr, "\n");
    va_end(vargs);
    exit(1);
}

void warn(const char* format, ...)
{
    va_list vargs;
    va_start(vargs, format);
    fprintf(stderr, "[RTGN WARNING] ");
    vfprintf(stderr, format, vargs);
    fprintf(stderr, "\n");
    va_end(vargs);
}
