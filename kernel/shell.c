#include "shell.h"
#include "vga.h"
#include "process.h"
#include "keyboard.h"

/* Declare the correct function name from keyboard.c */
extern char kb_getchar(void);

static int k_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

static int k_strncmp(const char *s1, const char *s2, int n) {
    while (n > 0 && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }
    if (n == 0) return 0;
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

void cmd_ps(void) {
    vga_puts("PID   STATE\n");
    vga_puts("------------\n");
    
    for (int i = 0; i < num_processes; i++) {
        char pid_str[3];
        int pid = processes[i].pid;
        
        if (pid < 10) {
            pid_str[0] = pid + '0';
            pid_str[1] = '\0';
        } else {
            pid_str[0] = (pid / 10) + '0';
            pid_str[1] = (pid % 10) + '0';
            pid_str[2] = '\0';
        }
        
        vga_puts(pid_str);
        vga_puts("     ");
        
        if (processes[i].state == PROCESS_READY) vga_puts("READY\n");
        else if (processes[i].state == PROCESS_RUNNING) vga_puts("RUN\n");
        else vga_puts("BLOCKED\n");
    }
}

void execute_command(char *cmd) {
    if (k_strcmp(cmd, "help") == 0) {
        vga_puts("Available commands:\n");
        vga_puts("  help  - Display this help message\n");
        vga_puts("  about - Show system details\n");
        vga_puts("  ps    - List active processes and states\n");
        vga_puts("  clear - Clear the screen\n");
        vga_puts("  echo  - Print text to screen\n");
    } 
    else if (k_strcmp(cmd, "about") == 0) {
        vga_puts("SENG OS - Bare-metal 32-bit Protected Mode Kernel\n");
        vga_puts("Built by students, for students.\n");
    } 
    else if (k_strcmp(cmd, "clear") == 0) {
        vga_clear(0);
    } 
    else if (k_strncmp(cmd, "echo ", 5) == 0) {
        vga_puts(cmd + 5);
        vga_puts("\n");
    } 
    else if (k_strcmp(cmd, "echo") == 0) {
        vga_puts("\n");
    } 
    else if (k_strcmp(cmd, "ps") == 0) {
        cmd_ps();
    } 
    else if (cmd[0] != '\0') {
        vga_puts("Unknown command: ");
        vga_puts(cmd);
        vga_puts("\n");
    }
}

void shell_run(void) {
    char buffer[256];
    int idx = 0;

    vga_puts("\nKernel Shell ready. Type 'help' for commands.\nksh> ");

    while (1) {
        /* Calling the corrected function name from keyboard.c */
        char c = kb_getchar(); 
        if (c == '\n' || c == '\r') {
            vga_puts("\n");
            buffer[idx] = '\0';
            execute_command(buffer);
            idx = 0;
            vga_puts("ksh> ");
        } else if (c == '\b') {
            if (idx > 0) {
                idx--;
                vga_puts("\b \b");
            }
        } else if (c != 0) {
            if (idx < 255) {
                buffer[idx++] = c;
                char str[2] = {c, '\0'};
                vga_puts(str);
            }
        }
    }
}