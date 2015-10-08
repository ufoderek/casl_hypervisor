#ifndef _MONITOR_H_
#define _MONITOR_H_

#include <stdint.h>

typedef enum monitor_events
{
    TRAP_WFI = 0,
    TRAP_CP15,
    TRAP_DABORT_VDEV,
    ALLOCATE_PAGE,
    TRAP_IRQ,
    VM_SWITCH,
    MON_EVENT_COUNT
} monitor_events;

typedef struct monitor
{
    uint32_t print_count;
    uint64_t events[MON_EVENT_COUNT];
} monitor;

void monitor_trigger(monitor* mon, monitor_events event);
uint64_t monitor_read(monitor* mon, monitor_events event);
void monitor_reset(monitor* mon);
void monitor_print(monitor* mon);

#endif

