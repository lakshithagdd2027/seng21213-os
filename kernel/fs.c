#include "../include/fs.h"
#include "../include/ramdisk.h"
#include "vga.h"

static int f_strcmp(const char *a, const char *b) {
    while (*a && (*a == *b)) { a++; b++; }
    return (uint8_t)*a - (uint8_t)*b;
}

static void f_strcpy(char *d, const char *s) {
    while(*s) *d++ = *s++;
    *d = 0;
}

uint8_t block_bitmap[32];
uint8_t inode_bitmap[16];
dir_entry_t root_dir[128];
inode_t inodes[128];

static int alloc_block() {
    for(int i=4; i<TOTAL_BLOCKS; i++) {
        if(!(block_bitmap[i/8] & (1<<(i%8)))) {
            block_bitmap[i/8] |= (1<<(i%8));
            return i;
        }
    }
    return -1;
}

static void free_block(int i) { block_bitmap[i/8] &= ~(1<<(i%8)); }

static int alloc_inode() {
    for(int i=1; i<128; i++) {
        if(!(inode_bitmap[i/8] & (1<<(i%8)))) {
            inode_bitmap[i/8] |= (1<<(i%8));
            return i;
        }
    }
    return -1;
}

static void free_inode(int i) { inode_bitmap[i/8] &= ~(1<<(i%8)); }

void fs_init(void) {
    ramdisk_init();
    for(int i=0; i<32; i++) block_bitmap[i] = 0;
    for(int i=0; i<16; i++) inode_bitmap[i] = 0;
    for(int i=0; i<4; i++) block_bitmap[0] |= (1<<i); 
    for(int i=0; i<128; i++) { root_dir[i].name[0] = 0; root_dir[i].inode = 0; }
}

void cmd_ls(void) {
    vga_puts_color("\n  Files in Root Directory:\n", VGA_LIGHT_CYAN, VGA_BLACK);
    vga_puts("  ------------------------\n");
    int count = 0;
    for(int i=0; i<128; i++) {
        if(root_dir[i].name[0] != 0) {
            vga_puts("  - "); vga_puts(root_dir[i].name); vga_puts("\n");
            count++;
        }
    }
    if (count == 0) vga_puts("  (Directory is empty)\n");
    vga_puts("\n");
}

void cmd_touch(const char *name) {
    for(int i=0; i<128; i++) {
        if(f_strcmp(root_dir[i].name, name) == 0) {
            vga_puts_color("  [Error] File already exists.\n", VGA_LIGHT_RED, VGA_BLACK);
            return;
        }
    }
    for(int i=0; i<128; i++) {
        if(root_dir[i].name[0] == 0) {
            int ino = alloc_inode();
            if(ino == -1) return;
            f_strcpy(root_dir[i].name, name);
            root_dir[i].inode = ino;
            inodes[ino].size = 0;
            vga_puts_color("  File created successfully.\n", VGA_LIGHT_GREEN, VGA_BLACK);
            return;
        }
    }
}

void cmd_rm(const char *name) {
    for(int i=0; i<128; i++) {
        if(f_strcmp(root_dir[i].name, name) == 0) {
            int ino = root_dir[i].inode;
            if (inodes[ino].size > 0) free_block(inodes[ino].blocks[0]);
            free_inode(ino);
            root_dir[i].name[0] = 0;
            vga_puts_color("  File deleted successfully.\n", VGA_LIGHT_GREEN, VGA_BLACK);
            return;
        }
    }
    vga_puts_color("  [Error] File not found.\n", VGA_LIGHT_RED, VGA_BLACK);
}

void cmd_write(const char *args) {
    char fname[28];
    int j=0;
    while(*args != ' ' && *args != 0 && j<27) fname[j++] = *args++;
    fname[j] = 0;
    if(*args == ' ') args++; 

    int ino = -1;
    for(int i=0; i<128; i++) {
        if(f_strcmp(root_dir[i].name, fname) == 0) { ino = root_dir[i].inode; break; }
    }
    if(ino == -1) { vga_puts_color("  [Error] File not found. Use 'touch'.\n", VGA_LIGHT_RED, VGA_BLACK); return; }

    int b = alloc_block();
    inodes[ino].blocks[0] = b;
    
    
    static char buf[4096];
    for(int i=0; i<4096; i++) buf[i] = 0;
    f_strcpy(buf, args);
    
    int len = 0; while(buf[len]) len++;
    inodes[ino].size = len;
    ramdisk_write(b, buf);
    vga_puts_color("  Data written successfully.\n", VGA_LIGHT_GREEN, VGA_BLACK);
}

void cmd_cat(const char *name) {
    for(int i=0; i<128; i++) {
        if(f_strcmp(root_dir[i].name, name) == 0) {
            int ino = root_dir[i].inode;
            if(inodes[ino].size == 0) { vga_puts("  (File is empty)\n"); return; }
            
            
            static char buf[4096];
            ramdisk_read(inodes[ino].blocks[0], buf);
            vga_puts_color("  Content: ", VGA_YELLOW, VGA_BLACK); vga_puts(buf); vga_puts("\n");
            return;
        }
    }
    vga_puts_color("  [Error] File not found.\n", VGA_LIGHT_RED, VGA_BLACK);
}