#include <hypervisor_call.h>
#include <rf.h>
#include <debug_utils.h>
#include <monitor.h>
#include <meteor_global_state.h>

void hypervisor_call(guest* vm, uint32_t imm16)
{
    rf_elr_hyp_write(rf_elr_hyp_read() - 4);

    switch(imm16)
    {
            //monitor print vm
        case 0x1:
            printmi(vm->id, d_hypervisor_call, "hypervisor call, monitor print vm");
            monitor_print(&vm->mon);
            break;
            //monitor print sys
        case 0x2:
            printmi(vm->id, d_hypervisor_call, "hypervisor call, monitor print sys");
            monitor_print(&sys_monitor);
            break;
        case 0x3:
            printmi(vm->id, d_hypervisor_call, "hypervisor call, monitor print all");
            printm(d_monitor, "================MON_SYS===================");
            monitor_print(&sys_monitor);
            printm(d_monitor, "================MON_VM====================");
            monitor_print(&vm->mon);
            printm(d_monitor, "==========================================");
            break;
        default:
            printm(d_hypervisor_call, "error hypervisor call, imm = %d", imm16);
            break;
    }
}

