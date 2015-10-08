#include <debug_utils.h>

#include <dev_gic2_dist.h>

#include <io.h>

void dev_gic2_dist_init(dev_gic2_dist* me, uint32_t base_addr)
{
    me->base_addr = base_addr;
    dev_gic2_dist_set_target(me, 0);
    dev_gic2_dist_set_enable(me, false);
}

void dev_gic2_dist_set_enable(dev_gic2_dist* me, bool en)
{
    if(en)
    {
        io_writew(me->base_addr, 1);
    }
    else
    {
        io_writew(me->base_addr, 0);
    }
}

void dev_gic2_dist_enable_irq(dev_gic2_dist* me, int irq)
{
    if((irq >= 0) && (irq <= 31))
    {
        io_writew(me->base_addr + 0x100, 1 << irq);
    }
    else if((irq >= 32) && (irq <= 63))
    {
        io_writew(me->base_addr + 0x104, 1 << irq);
    }
    else
    {
        printb(d_dev_gic2_dist, "error enable irq %d", irq);
    }
}

void dev_gic2_dist_disable_irq(dev_gic2_dist* me, int irq)
{
    if((irq >= 0) && (irq <= 31))
    {
        io_writew(me->base_addr + 0x180, 1 << irq);
    }
    else if((irq >= 32) && (irq <= 63))
    {
        io_writew(me->base_addr + 0x184, 1 << irq);
    }
    else
    {
        printb(d_dev_gic2_dist, "error disable irq %d", irq);
    }
}

void dev_gic2_dist_set_target(dev_gic2_dist* me, int cpu_if)
{
    int offset;

    if(cpu_if != 0)
    {
        printb(d_dev_gic2_dist, "error set cpu_if target to %d", cpu_if);
    }

    for(offset = 0x800; offset <= 0x8F8; offset += 4)
    {
        int n = (offset - 0x800) / 4;
        if((n >= 8) && (n <= 15))
        {
            io_writew(me->base_addr + offset, 0x01010101);
        }
    }
}

