#include <stdint.h>
#include <stdbool.h>

#include <bit_opt_v2l.h>
#include <cp15.h>
#include <debug_utils.h>
#include <dev_gic2_cpu_if.h>
#include <dev_timer.h>
#include <guest.h>
#include <guest_rpb.h>
#include <hypervisor_call.h>
#include <exception_handler.h>
#include <meteor_global_state.h>
#include <monitor.h>
#include <rf.h>
#include <scheduler.h>

void und_handler()
{
    printb(d_exception_handler, "hypervisor und exception");
}

void svc_handler()
{
    printb(d_exception_handler, "hypervisor svc exception");
}

void pabort_handler()
{
    printb(d_exception_handler, "hypervisor pabort");
}

void dabort_handler()
{
    printb(d_exception_handler, "hypervisor dabort");
}

void hyp_trap_handler(uint32_t sp)
{
    uint32_t hsr;
    uint32_t ec;
    uint32_t il;
    uint32_t iss;

    printd(d_exception_handler, "hyp_trap_abort, sp = 0x%X", sp);

    rf_elr_hyp_write(rf_elr_hyp_read() + 4);

    hsr = cp15_read(HSR);

    ec = get_field(hsr, 31, 26);
    il = get_bit(hsr, 25);
    iss = get_field(hsr, 24, 0);

    printd(d_exception_handler, "elr_hyp = 0x%X", rf_elr_hyp_read());
    printd(d_exception_handler, "HSR = 0x%X", hsr);
    printm_once(d_exception_handler, "hyp_trap_handler ignore checking IL");
    printd(d_exception_handler, "ec = 0x%X il = 0x%X, iss = 0x%X", ec, il, iss);

    if(vm[vm_id].type == Realview_PB)
    {
        switch(ec)
        {
            case 0x01:
                if(get_bit(iss, 0) == 0)
                {
                    monitor_trigger(&vm[vm_id].mon, TRAP_WFI);
                    printdi(vm_id, d_exception_handler, "hyp_trap wfi");
                    vm[vm_id].sched.wfi_state = WFI_In;
                    scheduler(sp);
                }
                else
                {
                    printb(d_exception_handler, "hyp_trap wfe");
                }
                break;
            case 0x3:
            case 0x4:
                //cp15, MCR or MRC
                //cp15, MCRR or MRRC
                monitor_trigger(&sys_monitor, TRAP_WFI);
                monitor_trigger(&vm[vm_id].mon, TRAP_CP15);
                guest_rpb_cp15_trap(&vm[vm_id], sp, iss);
            case 0x5:
                //cp14, MCR or MRC
                //ignore
                break;
            case 0x12:
                hypervisor_call(&vm[vm_id], iss & mask(15, 0));
                break;
            case 0x24:
                printd(d_exception_handler, "hyp_trap dabort route");
                guest_rpb_hyp_trap_dabort_handler(&vm[vm_id], sp, iss);
                break;
            case 0x25:
                printb(d_exception_handler, "hyp_trap dabort taken");
                guest_rpb_hyp_trap_dabort_handler(&vm[vm_id], sp, iss);
                break;
            default:
                printb(d_exception_handler, "hyp_trap error, ec=0x%X", ec);
                break;
        }
    }
    else
    {
        printb(d_exception_handler, "hypervisor hyp trap exception wrong vm type");
    }
}

void irq_handler(uint32_t sp)
{
    int pid;
    int target_vm_id;

    pid = dev_gic2_cpu_if_read_ack(&sys_gic_cpu_if);

    printd(d_exception_handler, "irq handler, pirq = %d , sp = 0x%X", pid, sp);

    switch(pid)
    {
        case 34:
            //timer interrupt for hypervisor
            dev_timer_clear_interrupt(&sys_timer, 0);
            dev_gic2_cpu_if_write_eoi(&sys_gic_cpu_if, 34);
            scheduler(sp);
            return;
        case 40:
        case 44:
        case 48:
            //for vm[0]
            target_vm_id = 0;
            break;
        case 41:
        case 45:
        case 49:
            //for vm[1]
            target_vm_id = 1;
            break;
        case 42:
        case 46:
        case 50:
            //for vm[2]
            target_vm_id = 2;
            break;
        case 43:
        case 47:
        case 51:
            //for vm[3]
            target_vm_id = 3;
            break;
        case 1023:
            //spurious interrupt
            return;
        default:
            printb(d_exception_handler, "unknown irq %d", pid);
            return;
    }

    monitor_trigger(&sys_monitor, TRAP_IRQ);

    if(vm[target_vm_id].type == Realview_PB)
    {
        //if target vm has dirty backup and not in WFI state, then it is active
        if(vm[target_vm_id].bk.dirty && (vm[target_vm_id].sched.wfi_state !=  WFI_In))
            //if(vm_id == target_vm_id)
        {
            guest_rpb_irq_handler(&vm[target_vm_id], sp, pid);
        }
        else
        {
            guest_rpb_irq_handler_suspend(&vm[target_vm_id], sp, pid);
        }
        if(vm[target_vm_id].sched.wfi_state ==  WFI_In)
        {
            vm[target_vm_id].sched.wfi_state = WFI_PendingIRQ;
        }
        if(scheduler_is_idle())
        {
            scheduler(sp);
        }
    }
    else
    {
        printb(d_exception_handler, "error guest type in irq_handler");
    }
}

void fiq_handler()
{
    printb(d_exception_handler, "hypervisor fiq handler");
}

