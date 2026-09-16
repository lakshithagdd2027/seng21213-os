#include "process.h"

pcb_t process_table[MAX_PROCESSES];
int process_count = 0;

void process_init(void) {
    process_count = 0;
}

int create_process(void (*entry_fn)()) {
    if (process_count >= MAX_PROCESSES) return -1;

    pcb_t *p = &process_table[process_count];
    p->pid = process_count; 
    p->state = PROCESS_READY;
    p->entry_point = entry_fn;

    uint32_t *stk = (uint32_t *)(p->stack + STACK_SIZE);

    *(--stk) = 0x0202;             // EFLAGS (Interrupts Enabled)
    *(--stk) = 0x08;               // CS
    *(--stk) = (uint32_t)entry_fn; // EIP

    // 8 General Purpose Registers pushed by PUSHAD
    for (int i = 0; i < 8; i++) {
        *(--stk) = 0;              
    }

    p->esp = (uint32_t)stk;
    process_count++;
    return p->pid;
}