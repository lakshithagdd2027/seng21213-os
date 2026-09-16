#include "../include/idt.h"

struct idt_entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct idt_entry idt[256];
struct idt_ptr idtr;

extern void isr_timer_stub(void);

void dummy_handler(void) {
    __asm__ __volatile__("iret");
}

void set_idt_gate(int n, uint32_t handler) {
    idt[n].base_low = handler & 0xFFFF;
    idt[n].selector = 0x08; 
    idt[n].zero = 0;
    idt[n].flags = 0x8E;   
    idt[n].base_high = (handler >> 16) & 0xFFFF;
}

void init_idt(void) {
    idtr.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtr.base = (uint32_t)&idt;

    for (int i = 0; i < 256; i++) {
        set_idt_gate(i, (uint32_t)dummy_handler);
    }

    set_idt_gate(32, (uint32_t)isr_timer_stub);

    __asm__ __volatile__("lidt %0" : : "m"(idtr));
}