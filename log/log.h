#ifndef __LOG_H
#define __LOG_H

#include <stdio.h>
#define LOG(...) printf(__VA_ARGS__);printf("\n")
// #define DEBUGLOG(...) printf(__VA_ARGS__);printf("\n")
#define DEBUGLOG(...)
#define HEXDUMP(...) hexdump(__VA_ARGS__);printf("\n");

void hexdump(void *buf, size_t bufsize);
#endif
