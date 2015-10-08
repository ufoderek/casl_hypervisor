#include <guest.h>
#include <debug_utils.h>
#include <io.h>
#include <cp15.h>
#include <dev_gic2_vctrl.h>
#include <dev_gic2_vcpu_if.h>
#include <meteor_global_state.h>

/*
stack diagram: (vector.s)

   R0_usr   SP-4x14
   R1_usr   SP-4x13
     ...
   R11_usr  SP-4x3
   R12_usr  SP-4x2
   LR_usr   SP-4x1

*/
uint32_t read_reg_from_stack(uint32_t sp, uint32_t rn)
{
    uint32_t offset = 0;

    if(rn <= 12) //R0_usr ~ R12_usr
    {
        offset = 4 * (14 - rn);
    }
    else if(rn == 14) //LR_usr
    {
        offset = 4;
    }
    else
    {
        printb(d_guest, "read_reg_in_stack error");
    }

    return io_readw(sp - offset);
}

void write_reg_to_stack(uint32_t sp, uint32_t rn, uint32_t value)
{
    uint32_t offset = 0;

    if(rn <= 12) //R0_usr ~ R12_usr
    {
        offset = 4 * (14 - rn);
    }
    else if(rn == 14) //LR_usr
    {
        offset = 4;
    }
    else
    {
        printb(d_guest, "read_reg_in_stack error");
    }

    io_writew(sp - offset, value);
}

void guest_backup_rf(guest* vm, uint32_t sp)
{
    int i;

    //backup R0_usr ~ R12_usr
    if(sp != 0)
    {
        for(i = 0; i <= 12; i++)
        {
            vm->bk.rf.r_usr[i] = read_reg_from_stack(sp, i);
            //printm(d_guest, "backup R%d_usr = 0x%X", i, vm->bk.rf.r_usr[i]);
        }
    }

    //backup SP
    asm volatile("mrs %0, SP_usr"
                 : "=r"(vm->bk.rf.sp_usr)
                );
    asm volatile("mrs %0, SP_svc"
                 : "=r"(vm->bk.rf.sp_svc)
                );
    asm volatile("mrs %0, SP_abt"
                 : "=r"(vm->bk.rf.sp_abt)
                );
    asm volatile("mrs %0, SP_und"
                 : "=r"(vm->bk.rf.sp_und)
                );
    asm volatile("mrs %0, SP_irq"
                 : "=r"(vm->bk.rf.sp_irq)
                );
    asm volatile("mrs %0, SP_fiq"
                 : "=r"(vm->bk.rf.sp_fiq)
                );
    /*
    printm(d_guest, "backup SP_usr = 0x%X", vm->bk.rf.sp_usr);
    printm(d_guest, "backup SP_svc = 0x%X", vm->bk.rf.sp_svc);
    printm(d_guest, "backup SP_abt = 0x%X", vm->bk.rf.sp_abt);
    printm(d_guest, "backup SP_und = 0x%X", vm->bk.rf.sp_und);
    printm(d_guest, "backup SP_irq = 0x%X", vm->bk.rf.sp_irq);
    printm(d_guest, "backup SP_fiq = 0x%X", vm->bk.rf.sp_fiq);
    */

    //backup LR
    if(sp != 0)
    {
        vm->bk.rf.lr_usr = read_reg_from_stack(sp, 14);
    }
    asm volatile("mrs %0, LR_svc"
                 : "=r"(vm->bk.rf.lr_svc)
                );
    asm volatile("mrs %0, LR_abt"
                 : "=r"(vm->bk.rf.lr_abt)
                );
    asm volatile("mrs %0, LR_und"
                 : "=r"(vm->bk.rf.lr_und)
                );
    asm volatile("mrs %0, LR_irq"
                 : "=r"(vm->bk.rf.lr_irq)
                );
    asm volatile("mrs %0, LR_fiq"
                 : "=r"(vm->bk.rf.lr_fiq)
                );
    /*
    printm(d_guest, "backup LR_usr = 0x%X", vm->bk.rf.lr_usr);
    printm(d_guest, "backup LR_svc = 0x%X", vm->bk.rf.lr_svc);
    printm(d_guest, "backup LR_abt = 0x%X", vm->bk.rf.lr_abt);
    printm(d_guest, "backup LR_und = 0x%X", vm->bk.rf.lr_und);
    printm(d_guest, "backup LR_irq = 0x%X", vm->bk.rf.lr_irq);
    printm(d_guest, "backup LR_fiq = 0x%X", vm->bk.rf.lr_fiq);
    */

    //backup SPSR
    asm volatile("mrs %0, SPSR_svc"
                 : "=r"(vm->bk.rf.spsr_svc)
                );
    asm volatile("mrs %0, SPSR_abt"
                 : "=r"(vm->bk.rf.spsr_abt)
                );
    asm volatile("mrs %0, SPSR_und"
                 : "=r"(vm->bk.rf.spsr_und)
                );
    asm volatile("mrs %0, SPSR_irq"
                 : "=r"(vm->bk.rf.spsr_irq)
                );
    asm volatile("mrs %0, SPSR_fiq"
                 : "=r"(vm->bk.rf.spsr_fiq)
                );

    //backup registers in hyp mode
    asm volatile("mrs %0, ELR_hyp"
                 : "=r"(vm->bk.rf.elr_hyp)
                );
    asm volatile("mrs %0, SPSR_hyp"
                 : "=r"(vm->bk.rf.spsr_hyp)
                );
}

void guest_restore_rf(guest* vm, uint32_t sp)
{
    int i;

    //restore R0_usr ~ R12_usr
    if(sp != 0)
    {
        for(i = 0; i <= 12; i++)
        {
            write_reg_to_stack(sp, i, vm->bk.rf.r_usr[i]);
            //printm(d_guest, "restore R%d_usr = 0x%X", i , read_reg_from_stack(sp, i));
        }
    }

    //restore SP
    asm volatile("msr SP_usr, %0"
                 :
                 : "r"(vm->bk.rf.sp_usr)
                );
    asm volatile("msr SP_svc, %0"
                 :
                 : "r"(vm->bk.rf.sp_svc)
                );
    asm volatile("msr SP_abt, %0"
                 :
                 : "r"(vm->bk.rf.sp_abt)
                );
    asm volatile("msr SP_und, %0"
                 :
                 : "r"(vm->bk.rf.sp_und)
                );
    asm volatile("msr SP_irq, %0"
                 :
                 : "r"(vm->bk.rf.sp_irq)
                );
    asm volatile("msr SP_fiq, %0"
                 :
                 : "r"(vm->bk.rf.sp_fiq)
                );
    /*
    printm(d_guest, "restore SP_usr = 0x%X", vm->bk.rf.sp_usr);
    printm(d_guest, "restore SP_svc = 0x%X", vm->bk.rf.sp_svc);
    printm(d_guest, "restore SP_abt = 0x%X", vm->bk.rf.sp_abt);
    printm(d_guest, "restore SP_und = 0x%X", vm->bk.rf.sp_und);
    printm(d_guest, "restore SP_irq = 0x%X", vm->bk.rf.sp_irq);
    printm(d_guest, "restore SP_fiq = 0x%X", vm->bk.rf.sp_fiq);
    */


    //restore LR
    if(sp != 0)
    {
        write_reg_to_stack(sp, 14, vm->bk.rf.lr_usr);
    }
    asm volatile("msr LR_svc, %0"
                 :
                 : "r"(vm->bk.rf.lr_svc)
                );
    asm volatile("msr LR_abt, %0"
                 :
                 : "r"(vm->bk.rf.lr_abt)
                );
    asm volatile("msr LR_und, %0"
                 :
                 : "r"(vm->bk.rf.lr_und)
                );
    asm volatile("msr LR_irq, %0"
                 :
                 : "r"(vm->bk.rf.lr_irq)
                );
    asm volatile("msr LR_fiq, %0"
                 :
                 : "r"(vm->bk.rf.lr_fiq)
                );
    /*
    printm(d_guest, "restore LR_usr = 0x%X", vm->bk.rf.lr_usr);
    printm(d_guest, "restore LR_svc = 0x%X", vm->bk.rf.lr_svc);
    printm(d_guest, "restore LR_abt = 0x%X", vm->bk.rf.lr_abt);
    printm(d_guest, "restore LR_und = 0x%X", vm->bk.rf.lr_und);
    printm(d_guest, "restore LR_irq = 0x%X", vm->bk.rf.lr_irq);
    printm(d_guest, "restore LR_fiq = 0x%X", vm->bk.rf.lr_fiq);
    */

    //restore SPSR
    asm volatile("msr SPSR_svc, %0"
                 :
                 : "r"(vm->bk.rf.spsr_svc)
                );
    asm volatile("msr SPSR_abt, %0"
                 :
                 : "r"(vm->bk.rf.spsr_abt)
                );
    asm volatile("msr SPSR_und, %0"
                 :
                 : "r"(vm->bk.rf.spsr_und)
                );
    asm volatile("msr SPSR_irq, %0"
                 :
                 : "r"(vm->bk.rf.spsr_irq)
                );
    asm volatile("msr SPSR_fiq, %0"
                 :
                 : "r"(vm->bk.rf.spsr_fiq)
                );

    //restore registers in hyp mode
    asm volatile("msr ELR_hyp, %0"
                 :
                 : "r"(vm->bk.rf.elr_hyp)
                );
    asm volatile("msr SPSR_hyp, %0"
                 :
                 : "r"(vm->bk.rf.spsr_hyp)
                );
}

void guest_backup_cp15(guest* vm)
{
    vm->bk.cp15.csselr = cp15_read(CSSELR);
    vm->bk.cp15.sctlr = cp15_read(SCTLR);
    vm->bk.cp15.ttbr0 = cp15_read(TTBR0);
    vm->bk.cp15.ttbr1 = cp15_read(TTBR1);
    vm->bk.cp15.ttbcr = cp15_read(TTBCR);
    vm->bk.cp15.dacr = cp15_read(DACR);
    vm->bk.cp15.dfsr = cp15_read(DFSR);
    vm->bk.cp15.ifsr = cp15_read(IFSR);
    vm->bk.cp15.dfar = cp15_read(DFAR);
    vm->bk.cp15.ifar = cp15_read(IFAR);
    vm->bk.cp15.prrr = cp15_read(PRRR);
    vm->bk.cp15.nmrr = cp15_read(NMRR);
    vm->bk.cp15.fcseidr = cp15_read(FCSEIDR);
    vm->bk.cp15.contextidr = cp15_read(CONTEXTIDR);
    vm->bk.cp15.tpidr_uro = cp15_read(TPIDR_URO);

    vm->bk.cp15.vttbr_lpae = cp15_read64(VTTBR_LPAE);
}

void guest_restore_cp15(guest* vm)
{
    cp15_write(CSSELR, vm->bk.cp15.csselr);
    cp15_write(SCTLR, vm->bk.cp15.sctlr);
    cp15_write(TTBR0, vm->bk.cp15.ttbr0);
    cp15_write(TTBR1, vm->bk.cp15.ttbr1);
    cp15_write(TTBCR, vm->bk.cp15.ttbcr);
    cp15_write(DACR, vm->bk.cp15.dacr);
    cp15_write(DFSR, vm->bk.cp15.dfsr);
    cp15_write(IFSR, vm->bk.cp15.ifsr);
    cp15_write(DFAR, vm->bk.cp15.dfar);
    cp15_write(IFAR, vm->bk.cp15.ifar);
    cp15_write(PRRR, vm->bk.cp15.prrr);
    cp15_write(NMRR, vm->bk.cp15.nmrr);
    cp15_write(FCSEIDR, vm->bk.cp15.fcseidr);
    cp15_write(CONTEXTIDR, vm->bk.cp15.contextidr);
    cp15_write(TPIDR_URO, vm->bk.cp15.tpidr_uro);

    cp15_write64(VTTBR_LPAE, vm->bk.cp15.vttbr_lpae);
}

void guest_backup_list(guest* vm)
{
    int i;

    for(i = 0; i < 64; i++)
    {
        vm->bk.list[i] = dev_gic2_vctrl_read_list(&sys_gic_vctrl, i);
    }
}

void guest_restore_list(guest* vm)
{
    int i;

    for(i = 0; i < 64; i++)
    {
        dev_gic2_vctrl_write_list_simple(&sys_gic_vctrl, i, vm->bk.list[i]);
    }
}

void guest_backup_gic2_vcpu_if(guest* vm)
{
    vm->bk.vcpu_if.ctlr = dev_gic2_vcpu_if_read_ctlr(&sys_gic_vcpu_if);
}

void guest_restore_gic2_vcpu_if(guest* vm)
{
    dev_gic2_vcpu_if_write_ctlr(&sys_gic_vcpu_if, vm->bk.vcpu_if.ctlr);
}

void guest_suspend(guest* vm, uint32_t sp)
{
    printd(d_guest, "starting backup vm: %d", vm->id);

    //if backup is dirty
    if(vm->bk.dirty)
    {
        guest_backup_rf(vm, sp);
        guest_backup_cp15(vm);
        guest_backup_gic2_vcpu_if(vm);
        guest_backup_list(vm);
    }

    vm->bk.dirty = false;

    printd(d_guest, "vm %d suspended, elr_hyp = 0x%X, vttbr = 0x%X", vm->id, vm->bk.rf.elr_hyp, (uint32_t)vm->bk.cp15.vttbr_lpae);
}

void guest_restore(guest* vm, uint32_t sp)
{
    printd(d_guest, "start restoring vm: %d", vm->id);

    //if backup is clean
    if(!vm->bk.dirty)
    {
        guest_restore_rf(vm, sp);
        guest_restore_cp15(vm);
        guest_restore_gic2_vcpu_if(vm);
        guest_restore_list(vm);

        vm_id = vm->id;
        cp15_write64(VTTBR_LPAE_VMID, vm_id);
    }
    else
    {
        printbi(vm->id, d_guest, "restore dirty backup");
    }

    vm->bk.dirty = true;

    printd(d_guest, "now resume vm %d, elr_hyp = 0x%X, vttbr = 0x%X", vm->id, vm->bk.rf.elr_hyp, (uint32_t)vm->bk.cp15.vttbr_lpae);
}

