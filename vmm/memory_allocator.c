#include <memory_allocator.h>
#include <pagetable.h>
#include <debug_utils.h>

void memory_allocator_init(memory_allocator* me, uint32_t mem_base)
{
    int i;

    for(i = 0; i < PAGE_COUNT; i++)
    {
        me->target_vm[i] = -1;
    }

    me->mem_base = mem_base;
    me->count = 0;
}

bool memory_allocator_page_exist(memory_allocator* me, guest* vm, uint32_t ipa, uint32_t high_ipa)
{
    int i;
    bool equal_vm_id;
    bool equal_ipa;
    bool equal_high_ipa;

    for(i = 0; i < PAGE_COUNT; i++)
    {
        equal_vm_id = me->target_vm[i] == vm->id;
        equal_ipa = me->ipa[i] == ipa;
        equal_high_ipa = me->high_ipa[i] == high_ipa;
        if(equal_vm_id && equal_ipa && equal_high_ipa)
        {
            return true;
        }
    }

    return false;
}

uint32_t memory_allocator_new_page(memory_allocator* me, guest* vm, uint32_t ipa, uint32_t high_ipa)
{
    static int last_i = 0;
    uint32_t pa;
    int i = 0;

    for(i = 0; i < PAGE_COUNT; i++)
    {
        //find next slot
        last_i++;
        if(last_i >= PAGE_COUNT)
        {
            last_i = 0;
        }

        //if slot is free, allocate it
        if(me->target_vm[last_i] == -1)
        {
            me->target_vm[last_i] = vm->id;
            me->ipa[last_i] = ipa;
            me->high_ipa[last_i] = high_ipa;
            me->count++;

            pa = me->mem_base + last_i * PAGE_SIZE;

            //printmi(vm->id, d_memory_allocator, "memory_allocator_new set ipa=%X pa=%X",ipa, me->mem_base+last_i*PAGE_SIZE);
            pagetable_set_page(&(vm->pt), ipa, pa);
            pagetable_set_page(&(vm->pt), high_ipa, pa);

            return pa;
        }
    }

    printbi(vm->id, d_memory_allocator, "memory_allocator_new() no free memory");

    return 0;
}

uint32_t memory_allocator_free_page(memory_allocator* me, guest* vm, uint32_t ipa, uint32_t high_ipa)
{
    printbi(vm->id, d_memory_allocator, "memory_allocator_free() not implemented yet");
    return 0;
}

