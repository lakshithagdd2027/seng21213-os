#include "process.h"
#include "scheduler.h"
#include "../include/pic.h"

extern pcb_t process_table[MAX_PROCESSES];
extern int process_count;

int current_process_index = 0;

uint32_t schedule(uint32_t current_esp) {
    if (process_count <= 1) {
        pic_send_eoi(0);
        return current_esp; 
    }

    process_table[current_process_index].esp = current_esp;
    
    if (process_table[current_process_index].state == PROCESS_RUNNING) {
        process_table[current_process_index].state = PROCESS_READY;
    }
    
    do {
        current_process_index = (current_process_index + 1) % process_count;
    } while (process_table[current_process_index].state != PROCESS_READY && process_table[current_process_index].state != PROCESS_RUNNING);

    process_table[current_process_index].state = PROCESS_RUNNING;

    pic_send_eoi(0);

    return process_table[current_process_index].esp;
}