#ifndef _GUEST_RPB_H_
#define _GUEST_RPB_H_

#include <guest.h>
#include <stdint.h>

void guest_rpb_init(guest* vm, int id, uint32_t mem_base_pa, uint32_t mem_base, uint32_t mem_size, dev_gic2_dist* sys_gic_dist);

void guest_rpb_init_mem(guest* vm);

void guest_rpb_init_mem_static(guest* vm);

void guest_rpb_init_pt(guest* vm, uint32_t pt_base, uint32_t uart_base,
                       uint32_t timer_0_base, uint32_t timer_2_base, uint32_t vcpu_if_base,
                       uint32_t black_hole_base);

void guest_rpb_init_pt_static(guest* vm, uint32_t pt_base, uint32_t uart_base,
                            uint32_t timer_0_base, uint32_t timer_2_base, uint32_t vcpu_if_base,
                            uint32_t black_hole_base);

void guest_rpb_init_virq(guest* vm, int uart_pid, int timer_0_pid, int timer_2_pid);

void guest_rpb_hyp_trap_dabort_handler(guest* vm, uint32_t sp, uint32_t iss);

void guest_rpb_irq_handler(guest* vm, uint32_t sp, int pid);

void guest_rpb_irq_handler_suspend(guest* vm, uint32_t sp, int pid);

void guest_rpb_cp15_trap(guest* vm, uint32_t sp, uint32_t iss);

#endif

