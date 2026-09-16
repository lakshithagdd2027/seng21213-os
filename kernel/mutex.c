#include "../include/mutex.h"

/* Assembly instructions to disable and enable interrupts */
static inline void cli(void) {
    __asm__ __volatile__("cli");
}

static inline void sti(void) {
    __asm__ __volatile__("sti");
}

void mutex_init(mutex_t *m) {
    m->locked = 0;
    m->owner = -1;
}

void mutex_lock(mutex_t *m) {
    while (1) {
        cli(); // Disable interrupts (Atomic check)
        
        if (m->locked == 0) {
            m->locked = 1;
            sti(); // Enable interrupts after acquiring the lock
            break;
        }
        
        sti(); // Enable interrupts to give other threads a chance if lock is unavailable
        
        /* 
         * Wait here until the lock is released (Blocking/Yielding). 
         * The timer interrupt will hand the CPU to another process via Round-robin.
         */
    }
}

void mutex_unlock(mutex_t *m) {
    cli();
    m->locked = 0;
    m->owner = -1;
    sti();
}