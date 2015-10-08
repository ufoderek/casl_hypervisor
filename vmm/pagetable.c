#include <bit_opt_v2l.h>
#include <debug_utils.h>
#include <io.h>
#include <pagetable.h>

void pagetable_init(pagetable_ld* pt, uint64_t base)
{
    int i;

    printd(d_pagetable, "starting pagetable_init");

    //init
    pt->l1_base = base;
    pt->l2_base = base + 0x1000;
    pt->l3_base = base + 0x5000;
    pt->page_count = 0;

    printd(d_pagetable, "l1_base = 0x%08X, l2_base = 0x%08X, l3_base = 0x%08X",
           (uint32_t)pt->l1_base, (uint32_t)pt->l2_base, (uint32_t)pt->l3_base);

    //there is 1 level 1 table witch has 4 entries
    for(i = 0; i < 4; i++)
    {
        uint64_t addr = pt->l1_base + i * 0x08;
        uint64_t l2_addr = pt->l2_base + i * 0x1000;

        uint64_t l1_desc;

        l1_desc = 0b11; //type = table
        set_field64(&l1_desc, 39, 12, l2_addr >> 12); //set next l2 table address

        io_writed(addr, l1_desc);
        printd(d_pagetable, "l1_desc = 0x%X:0x%08X%08X", (uint32_t)addr, (uint32_t)(l1_desc >> 32), (uint32_t)l1_desc);
    }

    //there are 4 level 2 tables, each with 512 entries
    for(i = 0; i < 4 * 512; i++)
    {
        uint64_t addr = pt->l2_base + i * 0x08;
        uint64_t l2_desc = 0; //type = invalid
        io_writed(addr, l2_desc);

        if((i % 512) == 0)
        {
            printd(d_pagetable, "l2_desc = 0x%X:0x%08X%08X", (uint32_t)addr, (uint32_t)(l2_desc >> 32), (uint32_t)l2_desc);
        }
    }

    //there are 8 level 3 tables, each with 512 entries
    for(i = 0; i < 8 * 512; i++)
    {
        uint64_t addr = pt->l3_base + i * 0x08;
        uint64_t l3_desc = 0; //type = invalid
        io_writed(addr, l3_desc);

        if((i % 512) == 0)
        {
            printd(d_pagetable, "l3_desc = 0x%X:0x%08X%08X", (uint32_t)addr, (uint32_t)(l3_desc >> 32), (uint32_t)l3_desc);
        }
    }

    printd(d_pagetable, "end of pagetable_init");
}

void pagetable_set_block(pagetable_ld* pt, uint64_t ipa, uint32_t pa)
{
    uint64_t l1_desc_addr = 0;
    uint64_t l1_desc = 0;
    uint64_t l2_desc_addr = 0;
    uint64_t l2_desc = 0;

    //finding address of level 1 descriptor
    set_field64(&l1_desc_addr, 39, 5, get_field64(pt->l1_base, 39, 5));
    set_field64(&l1_desc_addr, 4, 3, get_field64(ipa, 31, 30));

    //so we got level 1 descriptor
    l1_desc = io_readd(l1_desc_addr);

    printd(d_pagetable, "ipa 0x%08X%08X to pa 0x%X", (uint32_t)(ipa >> 32), (uint32_t)ipa, pa);
    //printd(d_pagetable, "l1_desc 0x%X:0x%08X%08X", (uint32_t)l1_desc_addr, (uint32_t)(l1_desc >> 32), (uint32_t)l1_desc);

    //finding address of level 2 descriptor
    set_field64(&l2_desc_addr, 39, 12, get_field64(l1_desc, 39, 12));
    set_field64(&l2_desc_addr, 11, 3, get_field64(ipa, 29, 21));
    //printd(d_pagetable, "ipa[29:21]=0x%X", get_field64(ipa, 29, 21) << 3);

    //set level 2 descriptor
    l2_desc = (1 << 10) | 1; //af+block
    set_field64(&l2_desc, 39, 21, pa >> 21);

    //store level 2 descriptor
    io_writed(l2_desc_addr, l2_desc);

    printd(d_pagetable, "l2_desc 0x%X:0x%08X%08X", (uint32_t)l2_desc_addr, (uint32_t)(l2_desc >> 32), (uint32_t)l2_desc);
}

void pagetable_set_page(pagetable_ld* pt, uint64_t ipa, uint32_t pa)
{
    uint64_t l1_desc_addr = 0;
    uint64_t l1_desc = 0;
    uint64_t l2_desc_addr = 0;
    uint64_t l2_desc = 0;
    uint64_t l3_desc_addr = 0;
    uint64_t l3_desc = 0;

    //get l1_desc
    set_field64(&l1_desc_addr, 39, 5, get_field64(pt->l1_base, 39, 5));
    set_field64(&l1_desc_addr, 4, 3, get_field64(ipa, 31, 30));
    l1_desc = io_readd(l1_desc_addr);

    printd(d_pagetable, "ipa 0x%X to pa 0x%X", (uint32_t)ipa, pa);
    printd(d_pagetable, "l1_desc 0x%X:0x%08X%08X", (uint32_t)l1_desc_addr, (uint32_t)(l1_desc >> 32), (uint32_t)l1_desc);

    //get l2_desc
    set_field64(&l2_desc_addr, 39, 12, get_field64(l1_desc, 39, 12));
    set_field64(&l2_desc_addr, 11, 3, get_field64(ipa, 29, 21));
    l2_desc = io_readd(l2_desc_addr);

    //if l2_desc is not table, give it a new page
    if(get_field64(l2_desc, 1, 0) != 0b11)
    {
        l3_desc_addr = pt->l3_base + (pt->page_count * 0x1000);
        printd(d_pagetable, "new l3_table at 0x%X", l3_desc_addr);
        pt->page_count++;

        //reset l2_desc
        l2_desc = 0b11;
        set_field64(&l2_desc, 39, 12, get_field64(l3_desc_addr, 39, 12));
        io_writed(l2_desc_addr, l2_desc);
    }

    //now l2_desc is valid
    printd(d_pagetable, "l2_desc 0x%X:0x%08X%08X", (uint32_t)l2_desc_addr, (uint32_t)(l2_desc >> 32), (uint32_t)l2_desc);

    //get l3_desc
    set_field64(&l3_desc_addr, 39, 12, get_field64(l2_desc, 39, 12));
    set_field64(&l3_desc_addr, 11, 3, get_field64(ipa, 20, 12));
    //l3_desc = io_readd(l3_desc_addr);

    //set l3_desc
    l3_desc = 1 << 10 | 0b11; //AF + block
    set_field64(&l3_desc, 39, 12, get_field64(pa, 39, 12));
    io_writed(l3_desc_addr, l3_desc);
    printd(d_pagetable, "l3_desc 0x%X:0x%08X%08X", (uint32_t)l3_desc_addr, (uint32_t)(l3_desc >> 32), (uint32_t)l3_desc);
}

