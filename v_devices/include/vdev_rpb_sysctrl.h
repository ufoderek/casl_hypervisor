#ifndef _VDEV_RPB_SYSCTRL_H_
#define _VDEV_RPB_SYSCTRL_H_

#include <stdint.h>

typedef struct vdev_rpb_sysctrl
{

} vdev_rpb_sysctrl;

uint32_t vdev_rpb_sysctrl_read(vdev_rpb_sysctrl* sysctlr, uint32_t offset);

void vdev_rpb_sysctrl_write(vdev_rpb_sysctrl* sysctlr, uint32_t offset, uint32_t data);

#endif
