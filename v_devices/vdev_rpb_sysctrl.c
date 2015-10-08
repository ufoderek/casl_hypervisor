#include <debug_utils.h>

#include <vdev_rpb_sysctrl.h>

uint32_t vdev_rpb_sysctrl_read(vdev_rpb_sysctrl* sysctrl, uint32_t offset)
{
    uint32_t data = 0;

    switch(offset)
    {
        case 0x0000:
            data = 0;
            break;
        case 0x0fe0:
            data = 0x80;
            break;
        case 0x0fe4:
            data = 0x11;
            break;
        case 0x0fe8:
            data = 0x04;
            break;
        case 0x0fec:
            data = 0x00;
            break;
        case 0x0ff0:
            data = 0x0d;
            break;
        case 0x0ff4:
            data = 0xf0;
            break;
        case 0x0ff8:
            data = 0x05;
            break;
        case 0x0ffc:
            data = 0xb1;
            break;
        default:
            printb(d_vdev_rpb_sysctrl, "read error, offset = 0x%X", offset);
    }

    return data;
}

void vdev_rpb_sysctrl_write(vdev_rpb_sysctrl* sysctrl, uint32_t offset, uint32_t data)
{
    switch(offset)
    {
        case 0x00:
            break;
        default:
            printb(d_vdev_rpb_sysctrl, "write 0x%X:0x%X error", offset, data);
            break;
    }
}

