#include <debug_utils.h>

#include <io.h>
#include <bit_opt_v2l.h>

#include <dev_gic2_vctrl.h>

void dev_gic2_vctrl_init(dev_gic2_vctrl* me, uint32_t base_addr)
{
    me->base_addr = base_addr;
}

void dev_gic2_vctrl_set_enable(dev_gic2_vctrl* me, bool en)
{
    io_writew(me->base_addr, en ? 1 : 0);
}

void dev_gic2_vctrl_write_list(dev_gic2_vctrl* me, int n, bool hw, list_reg_state state, int pid, int vid)
{
    uint32_t reg = 0;

    set_bit(&reg, 1, hw);
    set_field(&reg, 29, 28, state);
    set_field(&reg, 19, 10, pid);
    set_field(&reg, 9, 0, vid);

    printd(d_dev_gic2_vctrl, "write list, pid = %d, vid = %d", pid, vid);
    io_writew(me->base_addr + 0x100 + n * 4, reg);
}

void dev_gic2_vctrl_write_list_simple(dev_gic2_vctrl* me, int n, uint32_t value)
{
    io_writew(me->base_addr + 0x100 + n * 4, value);
}

uint32_t dev_gic2_vctrl_read_list(dev_gic2_vctrl* me, int n)
{
    return io_readw(me->base_addr + 0x100 + n * 4);
}

void dev_gic2_vctrl_clear_list(dev_gic2_vctrl* me)
{
    int i;

    for(i = 0; i < 64; i++)
    {
        dev_gic2_vctrl_write_list(me, i, false, LR_Invalid, 0, 0);
    }
}

