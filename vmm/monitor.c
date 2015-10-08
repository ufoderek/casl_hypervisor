#include <monitor.h>
#include <meteor_global_state.h>
#include <debug_utils.h>
#include <bit_opt_v2l.h>


void monitor_trigger(monitor* mon, monitor_events event)
{
    mon->events[event]++;
}

uint64_t monitor_read(monitor* mon, monitor_events event)
{
    return mon->events[event];
}

void monitor_reset(monitor* mon)
{
    int i;

    mon->print_count = 0;
    for(i = 0; i < MON_EVENT_COUNT; i++)
    {
        mon->events[i] = 0;
    }
}

void monitor_print(monitor* mon)
{
    printm(d_monitor, "print count = %d", mon->print_count++);
    printm(d_monitor, "trap wfi = 0x%08X%08X", (uint32_t)(mon->events[TRAP_WFI] >> 32), (uint32_t)mon->events[TRAP_WFI]&mask(31, 0));
    printm(d_monitor, "trap cp15 = 0x%08X%08X", (uint32_t)(mon->events[TRAP_CP15] >> 32), (uint32_t)mon->events[TRAP_CP15]&mask(31, 0));
    printm(d_monitor, "trap dabort vdev = 0x%08X%08X", (uint32_t)(mon->events[TRAP_DABORT_VDEV] >> 32), (uint32_t)mon->events[TRAP_DABORT_VDEV]&mask(31, 0));
    printm(d_monitor, "trap irq = 0x%08X%08X", (uint32_t)(mon->events[TRAP_IRQ] >> 32), (uint32_t)mon->events[TRAP_IRQ]&mask(31, 0));
    printm(d_monitor, "vm switch = 0x%08X%08X", (uint32_t)(mon->events[VM_SWITCH] >> 32), (uint32_t)mon->events[VM_SWITCH]&mask(31, 0));
}

