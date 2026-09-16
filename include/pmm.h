#ifndef PMM_H
#define PMM_H

#include "types.h"

void pmm_init(void);
void* pmm_alloc_frame(void);
void pmm_free_frame(void* paddr);
uint32_t pmm_get_total_memory(void);
uint32_t pmm_get_used_memory(void);

#endif