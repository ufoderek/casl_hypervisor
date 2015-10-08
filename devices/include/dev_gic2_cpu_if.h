#ifndef _DEV_GIC2_CPU_IF_H_
#define _DEV_GIC2_CPU_IF_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct dev_gic2_cpu_if
{
    uint32_t base_addr;
} dev_gic2_cpu_if;

void dev_gic2_cpu_if_init(dev_gic2_cpu_if* me, uint32_t base_addr);

void dev_gic2_cpu_if_set_enable(dev_gic2_cpu_if* me, bool en);

void dev_gic2_cpu_if_write_eoi(dev_gic2_cpu_if* me, uint32_t id);

uint32_t dev_gic2_cpu_if_read_ack(dev_gic2_cpu_if* me);

#endif
