#include <debug_utils.h>
#include <meteor_global_state.h>

#include <vdev_rpb_sysreg.h>
#include <dev_sys_ctrl.h>

uint32_t vdev_rpb_sysreg_read(vdev_rpb_sysreg* sysreg, uint32_t offset)
{
    switch(offset)
    {
        case 0x24:
            printd(d_vdev_rpb_sysreg, "100Hz Counter = 0x%X", dev_sys_ctrl_100counter(&sys_ctrl));
            return dev_sys_ctrl_100counter(&sys_ctrl);
            break;
        case 0x5C:
            printd(d_vdev_rpb_sysreg, "24MHz Counter = 0x%X", dev_sys_ctrl_24Mcounter(&sys_ctrl));
            return dev_sys_ctrl_24Mcounter(&sys_ctrl);
            break;
        default:
            printb(d_vdev_rpb_sysreg, "read error, offset = 0x%X", offset);
            break;
    }

    return 0;
}

void vdev_rpb_sysreg_write(vdev_rpb_sysreg* sysreg, uint32_t offset, uint32_t data)
{
    printb(d_vdev_rpb_sysreg, "write 0x%X:0x%X error", offset, data);
}

