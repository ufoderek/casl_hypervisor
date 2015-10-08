#include <stdbool.h>

#include <dev_gic2_dist.h>
#include <dev_gic2_cpu_if.h>
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

int vm_id;
guest vm[4];
memory_allocator sys_mem_allocator;

monitor sys_monitor;

dev_uart sys_uart;
dev_rom rom;
dev_semihost sys_semihost;
dev_gic2_dist sys_gic_dist;
dev_gic2_cpu_if sys_gic_cpu_if;
dev_gic2_vctrl sys_gic_vctrl;
dev_gic2_vcpu_if sys_gic_vcpu_if;
dev_timer sys_timer;
dev_sys_ctrl sys_ctrl;

