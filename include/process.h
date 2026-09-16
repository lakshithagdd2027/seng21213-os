#ifndef PROCESS_H
#define PROCESS_H

#include "types.h"

#define MAX_PROCESSES 16
#define STACK_SIZE    4096

typedef enum {
    PROCESS_UNUSED = 0,
    PROCESS_READY,
    PROCESS_RUNNING,
    PROCESS_BLOCKED,
    PROCESS_TERMINATED
} process_state_t;

typedef struct pcb {
    uint32_t pid;
    uint32_t esp;               /* Saved kernel stack pointer */
    process_state_t state;
    void (*entry_point)(void);  /* Entry function pointer */
    uint8_t stack[STACK_SIZE];  /* Dedicated kernel stack */
} pcb_t;

/* Function Declarations */
void process_init(void);
int create_process(void (*entry)(void));

#endif /* PROCESS_H */