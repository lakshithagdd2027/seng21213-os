#include "../include/thread.h"
#include "../include/process.h"

extern pcb_t process_table[MAX_PROCESSES];
extern int process_count;

int thread_create(void (*fn)(void*), void *arg) {
    if (process_count >= MAX_PROCESSES) return -1;

    pcb_t *p = &process_table[process_count];
    p->pid = process_count;
    p->state = PROCESS_READY;
    p->entry_point = (void (*)())fn;

    uint32_t *stk = (uint32_t *)(p->stack + STACK_SIZE);

    /* C Calling Convention: Push argument and dummy return address */
    *(--stk) = (uint32_t)arg;      // Argument for the thread function
    *(--stk) = 0x00000000;         // Fake return address 

    /* Interrupt frame for context switch */
    *(--stk) = 0x0202;             // EFLAGS (Interrupts Enabled)
    *(--stk) = 0x08;               // CS
    *(--stk) = (uint32_t)fn;       // EIP

    /* Push 8 empty values for general purpose registers (popad) */
    for (int i = 0; i < 8; i++) {
        *(--stk) = 0;
    }

    p->esp = (uint32_t)stk;
    process_count++;
    
    return p->pid;
}