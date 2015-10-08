#include <vdev_timer.h>
#include <debug_utils.h>

void vdev_timer_init(vdev_timer* me)
{
    int i = 0;

    for(i = 0; i < 2; i++)
    {
        me->load[i] = 0;
        me->value[i] = 0;
    }
}

uint32_t vdev_timer_read(vdev_timer* me, uint32_t offset)
{
    switch(offset)
    {
        case 0x0000:
            return me->load[0];
            break;
        case 0x0004:
            return me->value[0];
            break;
        case 0x0020:
            return me->load[1];
            break;
        case 0x0024:
            return me->value[1];
            break;
        default:
            printb(d_vdev_timer, "read 0x%X error");
            break;
    }

    return 0;
}

void vdev_timer_write(vdev_timer* me, uint32_t offset, uint32_t data)
{
    switch(offset)
    {
        case 0x0000:
            me->load[0] = data;
            me->value[0] = data;
            break;
        case 0x0004:
            break;
        case 0x0008:
            break;
        case 0x000c:
            break;
        case 0x0020:
            me->load[1] = data;
            me->value[1] = data;
        case 0x0024:
            break;
        case 0x0028:
            break;
        case 0x002c:
            break;
        default:
            printb(d_vdev_timer, "write 0x%X:0x%X error", offset, data);
            break;
    }
}

