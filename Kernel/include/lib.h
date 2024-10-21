#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void *my_memset(void * destination, int32_t character, uint64_t length);
void *my_memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

#endif