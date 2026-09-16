#include "pmm.h"
#include "types.h"

#define PAGE_SIZE 4096
#define BLOCKS_PER_BYTE 8
#define MEMORY_SIZE (128 * 1024 * 1024) // Assuming 128 MB total memory
#define BITMAP_SIZE (MEMORY_SIZE / PAGE_SIZE / BLOCKS_PER_BYTE)

static uint8_t memory_bitmap[BITMAP_SIZE] = {0};

static void bitmap_set(uint32_t bit) {
    memory_bitmap[bit / 8] |= (1 << (bit % 8));
}

static void bitmap_clear(uint32_t bit) {
    memory_bitmap[bit / 8] &= ~(1 << (bit % 8));
}

static int bitmap_test(uint32_t bit) {
    return memory_bitmap[bit / 8] & (1 << (bit % 8));
}

void pmm_init(void) {
    // Parse the BIOS E820 memory map here.
    // As a placeholder, we mark the first 1MB as used (reserved for BIOS/Kernel)
    for (uint32_t i = 0; i < (0x100000 / PAGE_SIZE); i++) {
        bitmap_set(i);
    }
}

void* pmm_alloc_frame(void) {
    uint32_t max_frames = MEMORY_SIZE / PAGE_SIZE;
    
    for (uint32_t i = 0; i < max_frames; i++) {
        if (!bitmap_test(i)) {
            bitmap_set(i);
            return (void*)(i * PAGE_SIZE);
        }
    }
    return 0; // Return 0 if out of memory
}

void pmm_free_frame(void* paddr) {
    uint32_t frame_index = (uint32_t)paddr / PAGE_SIZE;
    bitmap_clear(frame_index);
}

uint32_t pmm_get_total_memory(void) {
    return MEMORY_SIZE;
}

uint32_t pmm_get_used_memory(void) {
    uint32_t used_frames = 0;
    uint32_t max_frames = MEMORY_SIZE / PAGE_SIZE;
    
    for (uint32_t i = 0; i < max_frames; i++) {
        if (bitmap_test(i)) {
            used_frames++;
        }
    }
    return used_frames * PAGE_SIZE;
}