#ifndef _METEOR_GLOBAL_STATE_H_
#define _METEOR_GLOBAL_STATE_H_

#include <dev_gic2_cpu_if.h>
#include <dev_gic2_dist.h>
#include <dev_gic2_vctrl.h>
#include <dev_gic2_vcpu_if.h>
#include <dev_rom.h>
#include <dev_semihost.h>
#include <dev_timer.h>
#include <dev_uart.h>
#include <dev_sys_ctrl.h>
#include <memory_allocator.h>
#include <monitor.h>
#include <guest.h>

//global variables, I haven't done this for a long time.

extern int vm_id;
extern guest vm[4];
extern memory_allocator sys_mem_allocator;

extern monitor sys_monitor;
extern monitor vm_monitor[4];

extern dev_uart sys_uart;
extern dev_rom rom;
extern dev_semihost sys_semihost;
extern dev_gic2_dist sys_gic_dist;
extern dev_gic2_cpu_if sys_gic_cpu_if;
extern dev_gic2_vctrl sys_gic_vctrl;
extern dev_gic2_vcpu_if sys_gic_vcpu_if;
extern dev_timer sys_timer;
extern dev_sys_ctrl sys_ctrl;

#endif
