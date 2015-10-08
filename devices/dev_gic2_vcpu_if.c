#include <io.h>
#include <dev_gic2_vcpu_if.h>

void dev_gic2_vcpu_if_init(dev_gic2_vcpu_if* me, uint32_t base_address)
{
    me->base_address = base_address;
}

void dev_gic2_vcpu_if_write_ctlr(dev_gic2_vcpu_if* me, uint32_t reg)
{
    io_writew(me->base_address, reg);
}

uint32_t dev_gic2_vcpu_if_read_ctlr(dev_gic2_vcpu_if* me)
{
    return io_readw(me->base_address);
}

