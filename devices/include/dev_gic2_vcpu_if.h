#ifndef _DEV_GIC2_VCPU_IF_H_
#define _DEV_GIC2_VCPU_IF_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct dev_gic2_vcpu_if
{
    uint32_t base_address;
} dev_gic2_vcpu_if;

void dev_gic2_vcpu_if_init(dev_gic2_vcpu_if* me, uint32_t base_address);

void dev_gic2_vcpu_if_write_ctlr(dev_gic2_vcpu_if* me, uint32_t reg);

uint32_t dev_gic2_vcpu_if_read_ctlr(dev_gic2_vcpu_if* me);

#endif
