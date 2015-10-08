#ifndef _DEV_GIC2_DIST_H_
#define _DEV_GIC2_DIST_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct dev_gic2_dist
{
    uint32_t base_addr;
} dev_gic2_dist;

void dev_gic2_dist_init(dev_gic2_dist* me, uint32_t base_addr);

void dev_gic2_dist_set_enable(dev_gic2_dist* me, bool en);

void dev_gic2_dist_enable_irq(dev_gic2_dist* me, int irq);

void dev_gic2_dist_disable_irq(dev_gic2_dist* me, int irq);

void dev_gic2_dist_set_target(dev_gic2_dist* me, int cpu_if);

#endif
