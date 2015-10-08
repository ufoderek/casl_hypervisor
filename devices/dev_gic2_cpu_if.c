#include <debug_utils.h>

#include <dev_gic2_cpu_if.h>

#include <io.h>

void dev_gic2_cpu_if_init(dev_gic2_cpu_if* me, uint32_t base_addr)
{
    me->base_addr = base_addr;
}

void dev_gic2_cpu_if_set_enable(dev_gic2_cpu_if* me, bool en)
{
    io_writew(me->base_addr, en ? 1 : 0);
}

void dev_gic2_cpu_if_write_eoi(dev_gic2_cpu_if* me, uint32_t id)
{
    io_writew(me->base_addr + 0x10, id);
}

uint32_t dev_gic2_cpu_if_read_ack(dev_gic2_cpu_if* me)
{
    return io_readw(me->base_addr + 0x0C);
}

