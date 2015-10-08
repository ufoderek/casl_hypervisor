#ifndef _MEMORY_ALLOCATOR_H_
#define _MEMORY_ALLOCATOR_H_

#include <stdint.h>
#include <guest.h>

//both size in bytes
#define MEM_SIZE 4*128*1024*1024 // 4*128 MB
#define PAGE_SIZE 4*1024 // 4 KB

#define PAGE_COUNT (MEM_SIZE/1024) / (PAGE_SIZE/1024)

typedef struct memory_allocator
{
    int32_t target_vm[PAGE_COUNT];

    uint32_t ipa[PAGE_COUNT];
    uint32_t high_ipa[PAGE_COUNT];

    uint32_t mem_base;

    uint32_t count;
} memory_allocator;

void memory_allocator_init(memory_allocator* me, uint32_t mem_base);

bool memory_allocator_page_exist(memory_allocator* me, guest* vm, uint32_t ipa, uint32_t high_ipa);

uint32_t memory_allocator_new_page(memory_allocator* me, guest* vm, uint32_t ipa, uint32_t high_ipa);

uint32_t memory_allocator_free_page(memory_allocator* me, guest* vm, uint32_t ipa, uint32_t high_ipa);

#endif
