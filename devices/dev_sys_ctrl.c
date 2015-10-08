#include <dev_sys_ctrl.h>
#include <io.h>


void dev_sys_ctrl_init(dev_sys_ctrl* me, uint32_t base_addr_reg, uint32_t base_addr_ctrl)
{
    me->base_addr_reg = base_addr_reg;
    me->base_addr_ctrl = base_addr_ctrl;
}

uint32_t dev_sys_ctrl_100counter(dev_sys_ctrl* me)
{
    return io_readw(me->base_addr_reg + 0x24);
}

uint32_t dev_sys_ctrl_24Mcounter(dev_sys_ctrl* me)
{
    return io_readw(me->base_addr_reg + 0x5C);
}

