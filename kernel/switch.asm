global isr_timer_stub
extern schedule

isr_timer_stub:
    pushad          ; Save general purpose registers

    push esp        ; Pass ESP as argument to C function schedule()
    call schedule   ; Call C scheduler
    add esp, 4      ; Cleanup stack

    mov esp, eax    ; EAX holds the new stack pointer returned from schedule()

    popad           ; Restore general purpose registers for new process
    iretd           ; Return from interrupt