#include <debug_utils.h>
#include <meteor_global_state.h>
#include <monitor.h>
#include <scheduler.h>
#include <idle_thread.h>

static bool scheduler_idle = false;

void scheduler(uint32_t sp)
{
    scheduler_rr(sp);
}

void scheduler_rr(uint32_t sp)
{
    int i;
    int vm_count;
    int prev_vm_id;
    int next_vm_id;

    bool vm_valid;
    bool not_in_wfi;

    vm_count = sizeof(vm) / sizeof(guest);
    prev_vm_id = vm_id;
    next_vm_id = vm_id;

    //switch to other guests first if possible
    for(i = 0; i < vm_count; i++)
    {
        next_vm_id++;

        if(next_vm_id == vm_count)
        {
            next_vm_id = 0;
        }

        vm_valid = vm[next_vm_id].valid;
        not_in_wfi = vm[next_vm_id].sched.wfi_state != WFI_In;

        if(vm_valid && not_in_wfi)
        {
            //printm(d_scheduler, "vm switch, vm %d to vm %d", prev_vm_id, next_vm_id);

            //leaving from wfi
            if(vm[next_vm_id].sched.wfi_state == WFI_PendingIRQ)
            {
                printd(d_scheduler, "leave wfi");
                vm[next_vm_id].sched.wfi_state = WFI_Out;
                //scheduler_wait_gic_vcpu_if();
            }

            guest_suspend(&vm[prev_vm_id], sp);
            guest_restore(&vm[next_vm_id], sp);

            monitor_trigger(&sys_monitor, VM_SWITCH);

            scheduler_reset_timer();
            scheduler_leave_idle();
            return;
        }
    }

    //nothing to switch
    guest_suspend(&vm[prev_vm_id], sp);

    scheduler_reset_timer();
    scheduler_enter_idle();

    //never return
    idle_thread();
}

bool scheduler_is_idle()
{
    return scheduler_idle;
}

void scheduler_enter_idle()
{
    //printm(d_scheduler, "scheduler idle");

    scheduler_idle = true;
}

void scheduler_leave_idle()
{
    scheduler_idle = false;
}
void scheduler_reset_timer()
{
    /*
    40 MHz = 40 * 10^6 Hz
    = 4 * 10^7 Hz
    = 0.25 * 10^-7 s
    = 25 * 10^-9 s
    = 25 ns
    = 25 * 10^-9 s
    = 25 * 10^-6 * 10^-3 s
    = 25 * 10^-6 ms
    = 2.5 * 10^-5 ms
    = 0.000025 ms

    200 MHz = 200 * 10^6 Hz
    = 2 * 10^8 Hz
    = 0.5 * 10^-8 s
    = 5 * 10^-9 s
    = 5 ns

    1000 MHz = 1000 * 10^6 Hz
    = 1 * 10^9 Hz
    = 1 * 10^-9 s
    = 1 ns

    Linux context-switch conter value = 0x2710 = 100000
    = 25 * 100000 ns
    = 2500000 ns
    = 2.5 * 10^6 ns
    = 2.5 * 10^6 * 10^-9 s
    = 2.5 * 10^-3 s
    = 2.5 ms
    */

    const uint32_t vm_switch_ms = (uint32_t)(30.0 / 0.000025); //the timer is running at 40 MHz (0.000025 ms)

    dev_timer_set_load(&sys_timer, 0, vm_switch_ms);
}

void scheduler_wait_gic_vcpu_if()
{
    int i;
    for(i = 0; i < 300; i++)
    {
        asm volatile("nop");
    }
}

