#include "shell.h"
#include "pmm.h"
#include "vga.h" // Required for vga_puts

// If print_int is not defined in a header file, uncomment the line below:
// extern void print_int(int num);
extern void print_int(int num);

void cmd_meminfo(void) {
    uint32_t total_mb = pmm_get_total_memory() / (1024 * 1024);
    uint32_t used_mb = pmm_get_used_memory() / (1024 * 1024);
    uint32_t free_mb = total_mb - used_mb;

    vga_puts("\n  Physical Memory Information\n");
    vga_puts("  ---------------------------------------------\n");
    vga_puts("  Total : "); print_int(total_mb); vga_puts(" MB\n");
    vga_puts("  Used  : "); print_int(used_mb); vga_puts(" MB\n");
    vga_puts("  Free  : "); print_int(free_mb); vga_puts(" MB\n\n");
}

void cmd_test_pmm(void) {
    void* frames[100];
    
    vga_puts("\n  [Test] Allocating 100 memory frames...\n");
    for (int i = 0; i < 100; i++) {
        frames[i] = pmm_alloc_frame();
    }
    
    vga_puts("  [Test] Freeing 100 memory frames...\n");
    for (int i = 0; i < 100; i++) {
        pmm_free_frame(frames[i]);
    }
    
    vga_puts("  [Test] Complete. Run 'meminfo' to verify no leaks.\n\n");
}