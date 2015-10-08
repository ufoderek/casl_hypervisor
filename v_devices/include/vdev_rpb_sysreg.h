#ifndef _VDEV_RPB_SYSREG_H_
#define _VDEV_RPB_SYSREG_H_

#include <stdint.h>

typedef struct vdev_rpb_sysreg
{

} vdev_rpb_sysreg;

uint32_t vdev_rpb_sysreg_read(vdev_rpb_sysreg* sysctlr, uint32_t offset);

void vdev_rpb_sysreg_write(vdev_rpb_sysreg* sysctlr, uint32_t offset, uint32_t data);

#endif
