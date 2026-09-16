#ifndef MUTEX_H
#define MUTEX_H

#include <stdint.h>

/* Mutex structure */
typedef struct {
    int locked;
    int owner; 
} mutex_t;

/* Mutex functions */
void mutex_init(mutex_t *m);
void mutex_lock(mutex_t *m);
void mutex_unlock(mutex_t *m);

#endif