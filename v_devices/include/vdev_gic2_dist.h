#ifndef _VDEV_GIC2_DIST_H_
#define _VDEV_GIC2_DIST_H_

#include <stdint.h>
#include <stdbool.h>
#include <dev_gic2_dist.h>

typedef struct virq_state
{
    bool en;
    int pid;
} virq_state;

typedef struct vdev_gic2_dist
{
    bool en;
    virq_state virq[64];
    dev_gic2_dist* sys_gic_dist;
} vdev_gic2_dist;

void vdev_gic2_dist_init(vdev_gic2_dist* me, dev_gic2_dist* sys_gic_dist);

void vdev_gic2_dist_map_virq(vdev_gic2_dist* me, int vid, int pid);

int vdev_gic2_dist_irq_to_virq(vdev_gic2_dist* me, int pid);

uint32_t vdev_gic2_dist_read(vdev_gic2_dist* me, uint32_t offset);

void vdev_gic2_dist_write(vdev_gic2_dist* me, uint32_t offset, uint32_t data);

#endif
