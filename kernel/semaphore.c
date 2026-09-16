#include "../include/semaphore.h"

/* Assembly instructions to disable and enable interrupts */
static inline void cli(void) {
    __asm__ __volatile__("cli");
}

static inline void sti(void) {
    __asm__ __volatile__("sti");
}

void sem_init(semaphore_t *s, int value) {
    s->count = value;
}

void sem_wait(semaphore_t *s) {
    while (1) {
        cli(); // Disable interrupts (Atomic check)
        
        if (s->count > 0) {
            s->count--;
            sti(); // Enable interrupts after successfully decrementing
            break;
        }
        
        sti(); // Enable interrupts to yield CPU if count is 0
        
        /* 
         * Wait here until the semaphore count is greater than 0. 
         * Round-robin will schedule other threads in the meantime.
         */
    }
}

void sem_signal(semaphore_t *s) {
    cli();
    s->count++;
    sti();
}