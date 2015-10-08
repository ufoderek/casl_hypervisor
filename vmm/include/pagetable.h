#ifndef _PAGETABLE_H_
#define _PAGETABLE_H_

#include <stdint.h>

typedef struct pagetable_ld
{
    uint64_t l1_base;
    uint64_t l2_base;
    uint64_t l3_base;
    uint32_t page_count;
} pagetable_ld;


void pagetable_init(pagetable_ld* pt, uint64_t base);

void pagetable_set_block(pagetable_ld* pt, uint64_t ipa, uint32_t pa);
void pagetable_set_page(pagetable_ld* pt, uint64_t ipa, uint32_t pa);

#endif
