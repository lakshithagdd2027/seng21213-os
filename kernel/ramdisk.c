#include "../include/ramdisk.h"



static uint8_t *ramdisk = (uint8_t *)0x1000000;

void ramdisk_init(void) {
    for(int i = 0; i < RAMDISK_SIZE; i++) {
        ramdisk[i] = 0;
    }
}

void ramdisk_read(uint32_t block_num, void *buf) {
    if(block_num >= TOTAL_BLOCKS) return;
    uint8_t *d = (uint8_t *)buf;
    uint8_t *s = &ramdisk[block_num * BLOCK_SIZE];
    for(int i = 0; i < BLOCK_SIZE; i++) d[i] = s[i];
}

void ramdisk_write(uint32_t block_num, void *buf) {
    if(block_num >= TOTAL_BLOCKS) return;
    uint8_t *d = &ramdisk[block_num * BLOCK_SIZE];
    uint8_t *s = (uint8_t *)buf;
    for(int i = 0; i < BLOCK_SIZE; i++) d[i] = s[i];
}