#include <stdbool.h>
#include <stdlib.h>

#include <debug_utils.h>
#include <guest_rpb.h>
#include <meteor.h>
#include <meteor_global_state.h>
#include <scheduler.h>

void cleanup()
{
    printe("used pages: %d (%d KiB)", sys_mem_allocator.count, sys_mem_allocator.count * 4);
}

int main()
{
    debug_utils_init("meteor/");
    //debug_utils_setp(d_isr);
    //debug_utils_setp(d_guest_rpb);
    //debug_utils_setp(d_vdev_gic2_dist);
    //debug_utils_setp(d_dev_gic2_vctrl);

    meteor_devices_init();
    meteor_cp15_init();

    atexit(cleanup);

    guest_rpb_init(&vm[0], 0, 0x10000000, 0x0, 128 << 20, &sys_gic_dist);
    //guest_rpb_init_pt(&vm[0], 0x08000000, 0x60000000, 0x60004000, 0x60008000, 0x50003000, 0x90000000);
    //guest_rpb_init_mem(&vm[0]);
    guest_rpb_init_pt_static(&vm[0], 0x08000000, 0x60000000, 0x60004000, 0x60008000, 0x50003000, 0x90000000);
    guest_rpb_init_mem_static(&vm[0]);
    guest_rpb_init_virq(&vm[0], 40, 44, 48);

    guest_rpb_init(&vm[1], 1, 0x18000000, 0x0, 128 << 20, &sys_gic_dist);
    //guest_rpb_init_pt(&vm[1], 0x08100000, 0x60001000, 0x60005000, 0x60009000, 0x50003000, 0x90001000);
    //guest_rpb_init_mem(&vm[1]);
    guest_rpb_init_pt_static(&vm[1], 0x08100000, 0x60001000, 0x60005000, 0x60009000, 0x50003000, 0x90001000);
    guest_rpb_init_mem_static(&vm[1]);
    guest_rpb_init_virq(&vm[1], 41, 45, 49);

    guest_rpb_init(&vm[2], 2, 0x20000000, 0x0, 128 << 20, &sys_gic_dist);
    //guest_rpb_init_pt(&vm[2], 0x08200000, 0x60002000, 0x60006000, 0x6000A000, 0x50003000, 0x90002000);
    //guest_rpb_init_mem(&vm[2]);
    guest_rpb_init_pt_static(&vm[2], 0x08200000, 0x60002000, 0x60006000, 0x6000A000, 0x50003000, 0x90002000);
    guest_rpb_init_mem_static(&vm[2]);
    guest_rpb_init_virq(&vm[2], 42, 46, 50);

    guest_rpb_init(&vm[3], 3, 0x28000000, 0x0, 128 << 20, &sys_gic_dist);
    //guest_rpb_init_pt(&vm[3], 0x08300000, 0x60003000, 0x60007000, 0x6000B000, 0x50003000, 0x90003000);
    //guest_rpb_init_mem(&vm[3]);
    guest_rpb_init_pt_static(&vm[3], 0x08300000, 0x60003000, 0x60007000, 0x6000B000, 0x50003000, 0x90003000);
    guest_rpb_init_mem_static(&vm[3]);
    guest_rpb_init_virq(&vm[3], 43, 47, 51);

    guest_restore(&vm[0], 0);
    scheduler_leave_idle();
    scheduler_reset_timer();

    asm volatile("mov r0, #0");
    asm volatile("mov r1, #0");
    asm volatile("mov r2, #0");
    asm volatile("mov r3, #0");
    asm volatile("mov r4, #0");
    asm volatile("mov r5, #0");
    asm volatile("mov r6, #0");
    asm volatile("mov r7, #0");
    asm volatile("mov r8, #0");
    asm volatile("mov r9, #0");
    asm volatile("mov r10, #0");
    asm volatile("mov r11, #0");
    asm volatile("mov r12, #0");
    asm volatile("eret");

    return 0;
}

