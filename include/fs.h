#ifndef FS_H
#define FS_H

#include <stdint.h>

typedef struct {
    uint32_t size;
    uint32_t blocks[8]; // 8 x 4KB = 32KB Max size
} inode_t;

typedef struct {
    char name[28];
    uint32_t inode;
} dir_entry_t;

void fs_init(void);
void cmd_ls(void);
void cmd_touch(const char *name);
void cmd_cat(const char *name);
void cmd_write(const char *args);
void cmd_rm(const char *name);

#endif