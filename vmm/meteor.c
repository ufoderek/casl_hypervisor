#include <cp15.h>
#include <meteor.h>
#include <meteor_global_state.h>

void meteor_devices_init()
{
    //these two are used by newlib_stub.c
    extern dev_uart* newlib_uart;
    extern dev_semihost* newlib_semihost;

    dev_uart_init(&sys_uart, 0x50004000);
    newlib_uart = &sys_uart;

    dev_semihost_init(&sys_semihost, 0x70800000);
    newlib_semihost = &sys_semihost;

    dev_gic2_dist_init(&sys_gic_dist, 0x50000000);
    dev_gic2_dist_set_enable(&sys_gic_dist, true);

    dev_gic2_cpu_if_init(&sys_gic_cpu_if, 0x50001000);
    dev_gic2_cpu_if_set_enable(&sys_gic_cpu_if, true);

    dev_gic2_vctrl_init(&sys_gic_vctrl, 0x50002000);
    dev_gic2_vctrl_clear_list(&sys_gic_vctrl);
    dev_gic2_vctrl_set_enable(&sys_gic_vctrl, true);

    dev_gic2_vcpu_if_init(&sys_gic_vcpu_if, 0x50003000);

    dev_timer_init(&sys_timer, 0x50005000);
    dev_timer_set_wrapping(&sys_timer, 0, false);
    dev_timer_set_size(&sys_timer, 0, 32);
    dev_timer_set_prescale(&sys_timer, 0, 1);
    dev_timer_set_mode(&sys_timer, 0, TimerMode_Period);
    dev_timer_set_interrupt(&sys_timer, 0, true);
    dev_timer_set_enable(&sys_timer, 0, true);

    dev_sys_ctrl_init(&sys_ctrl, 0x50007000, 0x50008000);

    memory_allocator_init(&sys_mem_allocator, 0x10000000);

    monitor_reset(&sys_monitor);
}

void meteor_cp15_init()
{
    //init cp15
    cp15_write(HSCTLR, 0);
    cp15_write(HSCTLR_A, 1);
    cp15_write(HCR, 0x0);
    cp15_write(HCR_VM, 0x1);
    cp15_write(HCR_IMO, 0x1);
    cp15_write(HCR_TWI, 0x1);
    cp15_write(HVBAR, 0x0);
    cp15_write(VTCR, 0x0);
    cp15_write(VTCR_SL0, 0x1);
    cp15_write(VTCR_T0SZ, 0x0);
    cp15_write(HSTR_T7, 0x1);
    cp15_write(HSTR_T8, 0x1);
}

