#ifndef _VDEV_TIMER_H_
#define _VDEV_TIMER_H_

#include <stdint.h>

typedef struct vdev_timer
{
    uint32_t load[2];
    uint32_t value[2];
} vdev_timer;

void vdev_timer_init(vdev_timer* me);

uint32_t vdev_timer_read(vdev_timer* me, uint32_t offset);

void vdev_timer_write(vdev_timer* me, uint32_t offset, uint32_t data);

#endif
