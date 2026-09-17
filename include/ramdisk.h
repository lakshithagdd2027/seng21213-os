#ifndef RAMDISK_H
#define RAMDISK_H

#include <stdint.h>

#define RAMDISK_SIZE (1024 * 1024) // 1 MB RAM Disk
#define BLOCK_SIZE 4096            // 4 KB Blocks
#define TOTAL_BLOCKS (RAMDISK_SIZE / BLOCK_SIZE)

void ramdisk_init(void);
void ramdisk_read(uint32_t block_num, void *buf);
void ramdisk_write(uint32_t block_num, void *buf);

#endif