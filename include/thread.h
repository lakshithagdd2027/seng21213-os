#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>

/* Function prototype for creating a kernel thread */
int thread_create(void (*fn)(void*), void *arg);

#endif