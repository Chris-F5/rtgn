#ifndef RTGN_UTILS_H
#define RTGN_UTILS_H

#include <aio.h>

void* emalloc(size_t size);
void* erealloc(void* ptr, size_t size);
void die(const char* format, ...);
void warn(const char* format, ...);

#endif
