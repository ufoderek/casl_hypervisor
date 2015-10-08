#ifndef _DEV_SYS_CTRL_H_
#define _DEV_SYS_CTRL_H_

#include <stdint.h>

typedef struct dev_sys_ctrl
{
    uint32_t base_addr_reg;
    uint32_t base_addr_ctrl;
} dev_sys_ctrl;

void dev_sys_ctrl_init(dev_sys_ctrl* me, uint32_t base_addr_reg, uint32_t base_addr_ctrl);

uint32_t dev_sys_ctrl_100counter(dev_sys_ctrl* me);

uint32_t dev_sys_ctrl_24Mcounter(dev_sys_ctrl* me);

#endif
