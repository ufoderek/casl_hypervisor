#include <stdio.h>
#include <stdbool.h>

#include <io.h>
#include <rf.h>
#include <cp15.h>
#include <bit_opt_v2l.h>
#include <debug_utils.h>

#include <guest_rpb.h>
#include <meteor_global_state.h>
#include <monitor.h>

void guest_rpb_init(guest* vm, int id, uint32_t mem_base_pa, uint32_t mem_base, uint32_t mem_size, dev_gic2_dist* sys_gic_dist)
{
    //reset all fields to zero at first
    memset(vm, 0, sizeof(guest));

    vm->valid = true;
    vm->type = Realview_PB;
    vm->id = id;
    vm->mem_base_pa = mem_base_pa;
    vm->mem_base = mem_base;
    vm->mem_size = mem_size;
    vm->highmem_base = 0x70000000;
    vm->kernel_base = 0x00010000;
    vm->initrd_base = 0x00d00000;
    vm->atag_base = 0x00000100;

    vm->bk.dirty = false;
    vm->bk.rf.elr_hyp = 0x70000000;
    vm->bk.rf.spsr_hyp = 0x400001D3;
    vm->bk.cp15.sctlr = 0x00C50078;

    vm->sched.wfi_state = WFI_Out;

    vdev_gic2_dist_init(&vm->vdev_gic_dist, sys_gic_dist);
    vdev_timer_init(&vm->vdev_timer_2);
    monitor_reset(&vm->mon);

    printmi(vm->id, d_guest_rpb, "mem_base = 0x%X, mem_size = 0x%X, kernel_base = 0x%X", mem_base, mem_size, vm->kernel_base);
    printmi(vm->id, d_guest_rpb, "initrd_base = 0x%X, atag_base = 0x%X", vm->initrd_base, vm->atag_base);
}

void guest_rpb_init_mem(guest* vm)
{
    uint32_t i;
    int data;
    uint32_t kernel_size;
    uint32_t initrd_size;
    uint32_t ipa;
    uint32_t pa;
    FILE* fin;

    const uint32_t bootloader[] =
    {
        0xe3a00000, // mov     r0, #0
        0xe3a01069, // mov     r1, #0x??
        0xe3811c07, // orr     r1, r1, #0x??00
        0xe59f2000, // ldr     r2, [pc, #0]
        0xe59ff000, // ldr     pc, [pc, #0]
        0x70000100, // address of atag
        0x70010000  // kernel entry point
    };

    uint32_t atag[] =
    {
        0x5,        // ATAG_CORE size
        0x54410001, // ATAG_CORE
        0x1,
        0x1000,
        0x0,

        0x4,        // ATAG_MEM size*
        0x54410002, // ATAG_MEM
        vm->mem_size,  // ram size
        0x70000000, // entry point (also means memory base address here)

        0x4,        // ATAG_INITRD2 size
        0x54420005, // ATAG_INITRD2
        0x70d00000, // initrd addr
        0x0,        // initrd size = ?

        0x0000,     // ATAG_NONE size
        0x0000      // ATAG_NONE
    };

    printmi(vm->id, d_guest_rpb, "loading kernel...");
    fin = fopen("zImages/zImage.pb", "rb");
    ipa = vm->kernel_base;
    kernel_size = 0;
    do
    {
        pa = memory_allocator_new_page(&sys_mem_allocator, vm, ipa, ipa + 0x70000000);
        monitor_trigger(&(vm->mon), ALLOCATE_PAGE);
        printdi(vm->id, d_guest_rpb, "ipa=%X pa=%X", ipa, pa);

        for(i = 0; i < 4096; i++)
        {
            data = fgetc(fin);
            if(data == EOF)
            {
                break;
            }
            io_writeb(pa, data);
            kernel_size++;
            pa++;
            ipa++;
        }
    }
    while(data != EOF);
    fclose(fin);
    printmi(vm->id, d_guest_rpb, "kernel size = %d bytes", kernel_size);

    printmi(vm->id, d_guest_rpb, "loading initrd...");
    fin = fopen("initrds/initrd.gz", "rb");
    ipa = vm->initrd_base;
    initrd_size = 0;
    do
    {
        pa = memory_allocator_new_page(&sys_mem_allocator, vm, ipa, ipa + vm->highmem_base);
        monitor_trigger(&(vm->mon), ALLOCATE_PAGE);
        printdi(vm->id, d_guest_rpb, "ipa=%X pa=%X", ipa, pa);

        for(i = 0; i < 4096; i++)
        {
            data = fgetc(fin);
            if(data == EOF)
            {
                break;
            }
            io_writeb(pa, data);
            initrd_size++;
            pa++;
            ipa++;
        }
    }
    while(data != EOF);
    fclose(fin);
    atag[12] = initrd_size;
    printmi(vm->id, d_guest_rpb, "initrd size = %d bytes", initrd_size);

    ipa = 0;
    pa = memory_allocator_new_page(&sys_mem_allocator, vm, ipa, ipa + vm->highmem_base);
    monitor_trigger(&(vm->mon), ALLOCATE_PAGE);
    printmi(vm->id, d_guest_rpb, "ipa=%X pa=%X", ipa, pa);

    printmi(vm->id, d_guest_rpb, "loading bootloader");
    for(i = 0; i < sizeof(bootloader) / 4; i++)
    {
        io_writew(pa + 4 * i, bootloader[i]);
    }

    printmi(vm->id, d_guest_rpb, "loading atag");
    for(i = 0; i < sizeof(atag) / 4; i++)
    {
        io_writew(pa + vm->atag_base + 4 * i, atag[i]);
    }
}

void guest_rpb_init_mem_static(guest* vm)
{
    uint32_t i;
    int data;
    uint32_t kernel_size;
    uint32_t initrd_size;
    uint32_t pa;
    FILE* fin;

    const uint32_t bootloader[] =
    {
        0xe3a00000, // mov     r0, #0
        0xe3a01069, // mov     r1, #0x??
        0xe3811c07, // orr     r1, r1, #0x??00
        0xe59f2000, // ldr     r2, [pc, #0]
        0xe59ff000, // ldr     pc, [pc, #0]
        0x70000100, // address of atag
        0x70010000  // kernel entry point
    };

    uint32_t atag[] =
    {
        0x5,        // ATAG_CORE size
        0x54410001, // ATAG_CORE
        0x1,
        0x1000,
        0x0,

        0x4,        // ATAG_MEM size*
        0x54410002, // ATAG_MEM
        vm->mem_size,  // ram size
        0x70000000, // entry point (also means memory base address here)

        0x4,        // ATAG_INITRD2 size
        0x54420005, // ATAG_INITRD2
        0x70d00000, // initrd addr
        0x0,        // initrd size = ?

        0x0000,     // ATAG_NONE size
        0x0000      // ATAG_NONE
    };

    printmi(vm->id, d_guest_rpb, "loading kernel...");
    fin = fopen("zImages/zImage.pb", "rb");
    pa = vm->mem_base_pa + vm->kernel_base;
    kernel_size = 0;
    while(1)
    {
        data = fgetc(fin);
        if(data == EOF)
        {
            break;
        }
        io_writeb(pa, data);
        kernel_size++;
        pa++;
    }
    fclose(fin);
    printmi(vm->id, d_guest_rpb, "kernel size = %d bytes", kernel_size);

    printmi(vm->id, d_guest_rpb, "loading initrd...");
    fin = fopen("initrds/initrd.gz", "rb");
    pa = vm->mem_base_pa + vm->initrd_base;
    initrd_size = 0;
    while(1)
    {
        data = fgetc(fin);
        if(data == EOF)
        {
            break;
        }
        io_writeb(pa, data);
        initrd_size++;
        pa++;
    }
    fclose(fin);
    atag[12] = initrd_size;
    printmi(vm->id, d_guest_rpb, "initrd size = %d bytes", initrd_size);

    pa = vm->mem_base_pa; 
    printmi(vm->id, d_guest_rpb, "loading bootloader");
    for(i = 0; i < sizeof(bootloader) / 4; i++)
    {
        io_writew(pa + 4 * i, bootloader[i]);
    }

    printmi(vm->id, d_guest_rpb, "loading atag");
    pa = vm->mem_base_pa + vm->atag_base;
    for(i = 0; i < sizeof(atag) / 4; i++)
    {
        io_writew(pa + 4 * i, atag[i]);
    }
}

void guest_rpb_init_pt(guest* vm, uint32_t pt_base, uint32_t uart_base,
                       uint32_t timer_0_base, uint32_t timer_2_base, uint32_t vcpu_if_base,
                       uint32_t black_hole_base)
{
    pagetable_init(&(vm->pt), pt_base);

    //level 3 mmu maps memory using 4 KiB page
    pagetable_set_page(&vm->pt, 0x10009000, uart_base);
    pagetable_set_page(&vm->pt, 0x10011000, timer_0_base);
    pagetable_set_page(&vm->pt, 0x10012000, timer_2_base);
    pagetable_set_page(&vm->pt, 0x1E000000, vcpu_if_base);
    pagetable_set_page(&vm->pt, 0x20000000, black_hole_base);

    vm->bk.cp15.vttbr_lpae = pt_base;
}

void guest_rpb_init_pt_static(guest* vm, uint32_t pt_base, uint32_t uart_base,
                            uint32_t timer_0_base, uint32_t timer_2_base, uint32_t vcpu_if_base,
                            uint32_t black_hole_base)
{
    uint32_t offset;

    pagetable_init(&(vm->pt), pt_base);

    //level 2 mmu maps memory using 2 MiB block
    for(offset = 0; offset < (vm->mem_size); offset += 2 << 20)
    {
        pagetable_set_block(&vm->pt, offset, vm->mem_base_pa + offset);
        pagetable_set_block(&vm->pt, 0x70000000 + offset, vm->mem_base_pa + offset); //high mem
    }

    //level 3 mmu maps memory using 4 KiB page
    pagetable_set_page(&vm->pt, 0x10009000, uart_base);
    pagetable_set_page(&vm->pt, 0x10011000, timer_0_base);
    pagetable_set_page(&vm->pt, 0x10012000, timer_2_base);
    pagetable_set_page(&vm->pt, 0x1E000000, vcpu_if_base);
    pagetable_set_page(&vm->pt, 0x20000000, black_hole_base);

    vm->bk.cp15.vttbr_lpae = pt_base;
}

void guest_rpb_init_virq(guest* vm, int uart_pid, int timer_0_pid, int timer_2_pid)
{
    vdev_gic2_dist_map_virq(&vm->vdev_gic_dist, 44, uart_pid);
    vdev_gic2_dist_map_virq(&vm->vdev_gic_dist, 36, timer_0_pid);
    vdev_gic2_dist_map_virq(&vm->vdev_gic_dist, 37, timer_2_pid);
}

void guest_rpb_hyp_trap_dabort_handler(guest* vm, uint32_t sp, uint32_t iss)
{
    bool iss_valid;
    uint32_t hpfar;
    uint32_t hdfar;
    uint32_t ipa = 0;
    uint32_t base;
    uint32_t offset;
    uint32_t inst;
    bool is_load = true;

    uint32_t rt = 0;
    uint32_t rt_val = 0;


    iss_valid = get_bit(iss, 24);

    //va = hdfar
    hdfar = cp15_read(HDFAR);

    //ipa[31:12] = hpfar[31:4]
    hpfar = cp15_read(HPFAR);

    //this is ipa[31:12]
    ipa = (hpfar >> 4) << 12;

    //this is ipa[31:0]
    ipa |= hdfar & mask(11, 0);

    printdi(vm->id, d_guest_rpb, "iss_valid = %d, hdfar = 0x%X, hpfar = 0x%X, ipa = 0x%X", iss_valid, hdfar, hpfar, ipa);

    base = ipa & mask(31, 12);
    offset = ipa & mask(11, 0);

    //on-demand allocating memory

    /*
    #define memory_chunk 64 //KiB
    #define page_size 4096 //bytes
    #define page_count (memory_chunk / 4)
    */

    if((base >= 0) && (base <= (128 << 20)))
    {
        //int i;
        monitor_trigger(&(vm->mon), ALLOCATE_PAGE);
        memory_allocator_new_page(&sys_mem_allocator, vm, base , base + 0x70000000);
        /*
        for(i = 0; i < page_count; i++)
        {
            if(!memory_allocator_page_exist(&sys_mem_allocator, vm, base + i * page_size, base + 0x70000000 + i * page_size)){
                memory_allocator_new_page(&sys_mem_allocator, vm, base + i * page_size, base + 0x70000000 + i * page_size);
            }
        }
        */
        //you have to redo this instruction
        rf_elr_hyp_write(rf_elr_hyp_read() - 4);
        return;
    }
    else if((base >= 0x70000000) && (base <= (base + 0x70000000)))
    {
        //int i;
        monitor_trigger(&(vm->mon), ALLOCATE_PAGE);
        memory_allocator_new_page(&sys_mem_allocator, vm, base - 0x70000000 , base);
        /*
        for(i = 0; i < page_count; i++)
        {
            if(!memory_allocator_page_exist(&sys_mem_allocator, vm, base - 0x70000000 + i * page_size, base + i * page_size)){
                memory_allocator_new_page(&sys_mem_allocator, vm, base - 0x70000000 + i * page_size, base + i * page_size);
            }
        }
        */
        //you have to redo this instruction
        rf_elr_hyp_write(rf_elr_hyp_read() - 4);
        return;
    }

    monitor_trigger(&(vm->mon), TRAP_DABORT_VDEV);

    //other load/store
    //decode trapped instruction
    inst = cp15_read(HADFSR);

    if((get_field(inst, 27, 25) == 0b010) && (get_bit(inst, 22) == 0) && (get_bit(inst, 20) == 1))
    {
        //ldr_imm
        rt = get_field(inst, 15, 12);
        is_load = true;
    }
    else if((get_field(inst, 27, 24) == 0b0101) && (get_field(inst, 22, 16) == 0b0011111))
    {
        printbi(vm->id, d_guest_rpb, "ldr ltrl");
    }
    else if((get_field(inst, 27, 25) == 0b011) && (get_bit(inst, 22) == 0) && (get_bit(inst, 20) == 1) && (get_bit(inst, 4) == 0))
    {
        printbi(vm->id, d_guest_rpb, "ldr reg");
    }
    else if((get_field(inst, 27, 25) == 0b010) && (get_bit(inst, 22) == 0) && (get_bit(inst, 20) == 0))
    {
        //str_imm
        rt = get_field(inst, 15, 12);
        is_load = false;
    }
    else if((get_field(inst, 27, 25) == 0b011) && (get_bit(inst, 22) == 0) && (get_bit(inst, 20) == 0) && (get_bit(inst, 4) == 0))
    {
        //str_reg
        rt = get_field(inst, 15, 12);
        is_load = false;
    }
    else
    {
        printbi(vm->id, d_guest_rpb, "error decoding instruction:0x%X", inst);
    }

    if(base == 0x10000000)
    {
        printdi(vm->id, d_guest_rpb, "hyp trap sysreg");
        if(is_load)
        {
            rt_val = vdev_rpb_sysreg_read(&vm->vdev_sysreg, offset);
            write_reg_to_stack(sp, rt, rt_val);
        }
        else
        {
            rt_val =  read_reg_from_stack(sp, rt);
            vdev_rpb_sysreg_write(&vm->vdev_sysreg, offset, rt_val);
        }
    }
    else if(base == 0x10001000)
    {
        printdi(vm->id, d_guest_rpb, "hyp trap sysctrl");
        if(is_load)
        {
            rt_val = vdev_rpb_sysctrl_read(&vm->vdev_sysctrl, offset);
            write_reg_to_stack(sp, rt, rt_val);
        }
        else
        {
            rt_val =  read_reg_from_stack(sp, rt);
            vdev_rpb_sysctrl_write(&vm->vdev_sysctrl, offset, rt_val);
        }
    }
    else if(base == 0x10012000)
    {
        printdi(vm->id, d_guest_rpb, "hyp trap timer 2");
        if(is_load)
        {
            rt_val = vdev_timer_read(&vm->vdev_timer_2, offset);
            write_reg_to_stack(sp, rt, rt_val);
        }
        else
        {
            rt_val =  read_reg_from_stack(sp, rt);
            vdev_timer_write(&vm->vdev_timer_2, offset, rt_val);
        }
    }
    else if(base == 0x1E001000)
    {
        printdi(vm->id, d_guest_rpb, "hyp trap gic dist");
        if(is_load)
        {
            rt_val = vdev_gic2_dist_read(&vm->vdev_gic_dist, offset);
            write_reg_to_stack(sp, rt, rt_val);
        }
        else
        {
            rt_val =  read_reg_from_stack(sp, rt);
            vdev_gic2_dist_write(&vm->vdev_gic_dist, offset, rt_val);
        }
    }
    else
    {
        printbi(vm->id, d_guest_rpb, "error trapping hardware at ipa:0x%X", ipa);
    }
}

void guest_rpb_irq_handler(guest* vm, uint32_t sp, int pid)
{
    monitor_trigger(&(vm->mon), TRAP_IRQ);

    if(pid == 1023)
    {
        //printm(d_guest_rpb, "spurious interrupt");
    }
    else
    {
        int i;
        int vid;
        uint32_t list;

        vid = vdev_gic2_dist_irq_to_virq(&vm->vdev_gic_dist, pid);

        if(vid == -1)
        {
            printbi(vm->id, d_guest_rpb, "virtual interrupt %d not imple.", vid);
        }

        printdi(vm->id, d_guest_rpb, "irq handler, pid = %d, vid = %d", pid, vid);

        //use empty (invalid) list register
        for(i = 0; i < 64; i++)
        {
            list = dev_gic2_vctrl_read_list(&sys_gic_vctrl, i);
            if(get_field(list, 29, 28) == LR_Invalid)
            {
                //dev_gic2_vctrl_write_list(&sys_gic_vctrl, i, true, LR_Pending, pid, vid);
                set_bit(&list, 1, true);
                set_field(&list, 29, 28, LR_Pending);
                set_field(&list, 19, 10, pid);
                set_field(&list, 9, 0, vid);
                dev_gic2_vctrl_write_list_simple(&sys_gic_vctrl, i, list);

                return;
            }
        }

        printbi(vm->id, d_guest_rpb, "rpb_irq_handler no empty list register");

        //dev_gic2_vctrl_write_list(&sys_gic_vctrl, 0, true, LR_Pending, pid, vid);
    }
}

void guest_rpb_irq_handler_suspend(guest* vm, uint32_t sp, int pid)
{
    int vid;

    if(pid == 1023)
    {
        //printm(d_guest_rpb, "spurious interrupt");
    }
    else
    {
        int i;

        vid = vdev_gic2_dist_irq_to_virq(&vm->vdev_gic_dist, pid);

        if(vid == -1)
        {
            printbi(vm->id, d_guest_rpb, "virtual interrupt %d not imple.", vid);
        }

        printdi(vm->id, d_guest_rpb, "irq handler suspended, pid = %d, vid = %d", pid, vid);

        //find invalid list register
        for(i = 0; i < 64; i++)
        {
            if(get_field(vm->bk.list[i], 29, 28) == LR_Invalid)
            {
                set_bit(&vm->bk.list[i], 1, true);
                set_field(&vm->bk.list[i], 29, 28, LR_Pending);
                set_field(&vm->bk.list[i], 19, 10, pid);
                set_field(&vm->bk.list[i], 9, 0, vid);

                printdi(vm->id, d_guest_rpb, "end of rpb_irq_handler_suspend");
                return;
            }
        }

        printbi(vm->id, d_guest_rpb, "rpb_irq_handler_suspend no empty list register");
    }
}

void guest_rpb_cp15_trap(guest* vm, uint32_t sp, uint32_t iss)
{
    uint32_t crn = get_field(iss, 13, 10);
    uint32_t opc1 = get_field(iss, 16, 14);
    uint32_t crm = get_field(iss, 4, 1);
    uint32_t opc2 = get_field(iss, 19, 17);
    uint32_t rt = get_field(iss, 8, 5);
    bool is_read = get_bit(iss, 0);

    printmi_once(vm->id, d_guest_rpb, "please check and verify guest_rpb_cp15_trap");

    printdi(vm->id, d_guest_rpb, "crn=%d opc1=%d crm=%d opc2=%d rt=%d is_read=%s", crn, opc1, crm, opc2, rt, is_read ? "true" : "false");

    //world peace
}

