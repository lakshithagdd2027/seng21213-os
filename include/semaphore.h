#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>

/* Semaphore structure */
typedef struct {
    int count;
} semaphore_t;

/* Semaphore functions */
void sem_init(semaphore_t *s, int value);
void sem_wait(semaphore_t *s);
void sem_signal(semaphore_t *s);

#endif