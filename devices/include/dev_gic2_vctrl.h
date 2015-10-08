#ifndef _DEV_GIC2_VCTRL_H_
#define _DEV_GIC2_VCTRL_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum list_reg_state
{
    LR_Invalid = 0b00,
    LR_Pending = 0b01,
    LR_Active = 0b10,
    LR_PendingActive = 0b11
} list_reg_state;

typedef struct dev_gic2_vctrl
{
    uint32_t base_addr;

} dev_gic2_vctrl;

void dev_gic2_vctrl_init(dev_gic2_vctrl* me, uint32_t base_addr);

void dev_gic2_vctrl_set_enable(dev_gic2_vctrl* me, bool en);

void dev_gic2_vctrl_write_list(dev_gic2_vctrl* me, int n, bool hw, list_reg_state state, int pid, int vid);

void dev_gic2_vctrl_write_list_simple(dev_gic2_vctrl* me, int n, uint32_t value);

uint32_t dev_gic2_vctrl_read_list(dev_gic2_vctrl* me, int n);

void dev_gic2_vctrl_clear_list(dev_gic2_vctrl* me);

#endif
