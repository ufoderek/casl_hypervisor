#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <stdbool.h>
#include <stdint.h>

void scheduler(uint32_t sp);

void scheduler_rr(uint32_t sp);

bool scheduler_is_idle();

void scheduler_enter_idle();

void scheduler_leave_idle();

void scheduler_reset_timer();

void scheduler_wait_gic_vcpu_if();

#endif
