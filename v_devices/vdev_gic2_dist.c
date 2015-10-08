#include <debug_utils.h>
#include <bit_opt_v2l.h>

#include <vdev_gic2_dist.h>

void vdev_gic2_dist_init(vdev_gic2_dist* me, dev_gic2_dist* sys_gic_dist)
{
    int i;

    me->en = false;

    for(i = 0; i < 64; i++)
    {
        me->virq[i].en = false;
        me->virq[i].pid = -1;
    }

    me->sys_gic_dist = sys_gic_dist;
}

void vdev_gic2_dist_map_virq(vdev_gic2_dist* me, int vid, int pid)
{
    me->virq[vid].pid = pid;
    printd(d_vdev_gic2_dist, "map virq %d to irq %d", vid, pid);
}

int vdev_gic2_dist_irq_to_virq(vdev_gic2_dist* me, int pid)
{
    int i;
    for(i = 0; i < 64; i++)
    {
        if(me->virq[i].pid == pid)
        {
            return i;
        }
    }
    return -1;
}

uint32_t vdev_gic2_dist_read(vdev_gic2_dist* me, uint32_t offset)
{
    switch(offset)
    {
        case 0x04:
            return (0b11111 << 11) | (1 << 10) | (1 << 5) | (1 << 0);
            break;
        default:
            printb(d_vdev_gic2_dist, "read error, offset = 0x%X", offset);
            break;
    }

    return 0;
}

void vdev_gic2_dist_write(vdev_gic2_dist* me, uint32_t offset, uint32_t data)
{
    if(offset == 0x0)
    {
        bool group0_en;
        bool group1_en;

        group0_en = get_bit(data, 0);
        group1_en = get_bit(data, 1);

        if(group0_en && group1_en)
        {
            printb(d_vdev_gic2_dist, "enable group0,1 at the same time, not implemented.");
        }
        else if(group0_en || group1_en)
        {
            printd(d_vdev_gic2_dist, "vdev_gic2_dist enable");
            me->en = true;
        }
        else
        {
            printd(d_vdev_gic2_dist, "vdev_gic2_dist disable");
            me->en = false;
        }
    }
    else if(offset == 0x80)
    {
        //gic2D_IGROUPRn, ignore
    }
    else if((offset >= 0x100) && (offset <= 0x17C))
    {
        //gic2D_ISENABLERn
        //n = 0:  0 ~ 31 (banked)
        //n = 1: 32 ~ 63 (not banked)
        int i;
        int ir;
        uint32_t n = (offset - 0x100) / 4;
        uint32_t ir_base = n * 32;

        for(i = 0; i < 32; i++)
        {
            ir = ir_base + i;

            if(get_bit(data, i))
            {
                me->virq[ir].en = true;
                int pid = me->virq[ir].pid;
                if(pid != -1)
                {
                    dev_gic2_dist_enable_irq(me->sys_gic_dist, pid);
                    printd(d_vdev_gic2_dist, "enable virq forward, vid = %d, pid = %d", ir, pid);
                }
                else
                {
                    //printd(d_vdev_gic2_dist, "ignore enable virq forward %d", ir);
                }
            }
        }
    }
    else if((offset >= 0x180) && (offset <= 0x1FC))
    {
        //gic2D_ICENABLERn
        //n = 0:  0 ~ 31 (banked)
        //n = 1: 32 ~ 63 (not banked)
        int i;
        int ir;
        uint32_t n = (offset - 0x180) / 4;
        uint32_t ir_base = n * 32;

        for(i = 0; i < 32; i++)
        {
            ir = ir_base + i;

            if(get_bit(data, i))
            {
                me->virq[ir].en = false;
                int pid = me->virq[ir].pid;
                if(pid != -1)
                {
                    dev_gic2_dist_disable_irq(me->sys_gic_dist, pid);
                    printd(d_vdev_gic2_dist, "disable virq forward %d", ir);
                }
                else
                {
                    //printd(d_vdev_gic2_dist, "ignore disable virq forward %d", ir);
                }
            }
        }
    }
    else if((offset >= 0x400) && (offset <= 0x7F8))
    {
        //gic2D_IPRIORITYn, ignore
    }
    else if((offset >= 0x800) && (offset <= 0x8F8))
    {
        //gic2D_ITARGETSRn (for n=0~7, register is READ ONLY and banked for each processor)
        //n = 0:  0 ~  3
        //n = 1:  4 ~  7
        //n = 2:  8 ~  11
        int i;
        int ir;
        int cpu = 0;
        bool forward;
        uint32_t n = (offset - 0x800) / 4;
        uint32_t ir_base = n * 4;

        //make sure n > 7
        if(n <= 7)
        {
            printb(d_vdev_gic2_dist, "target register n = %d (< 7)", n);
        }

        //with n > 7, we now deal with shared interrupts
        for(i = 0; i < 4; i++)
        {
            ir = ir_base + i;

            forward = get_bit(data, cpu);
            printd(d_vdev_gic2_dist, "%s forwarding virq %d to cpu %d", forward ? "enable" : "disable", ir, cpu);

            data >>= 8;
        }
    }
    else if((offset >= 0xC00) && (offset <= 0xCFC))
    {
        //gic2D_ICFGRn, ignore
        //uint32_t n = (offset - 0xC00) / 4;

        //printd(d_gic22_dist, "write to gic2D_ICFGR");
    }
    else if(offset == 0xF00)
    {
        //gic2D_SGIR
        int cpu = 0;
        int cpu_count = 1;
        uint32_t target_list_filter = get_field(data, 25, 24);
        uint32_t cpu_target_list = get_field(data, 23, 16);
        uint32_t soft_int_id = get_field(data, 3, 0);

        if(target_list_filter == 0)
        {
            //send to specific cpus
            for(cpu = 0; cpu < cpu_count; cpu++)
            {
                if(get_bit(cpu_target_list, cpu))
                {
                    //ir_infos[cpu][soft_int_id]->set_pending();
                    printb(d_vdev_gic2_dist, "send spi[%d] to cpu[%d]", soft_int_id, cpu);
                }
            }
        }
        else if(target_list_filter == 1)
        {
            //send to other cpus
            printb(d_vdev_gic2_dist, "send to other cpus");
        }
        else if(target_list_filter == 2)
        {
            //send to itself
            printb(d_vdev_gic2_dist, "send to iteself");
        }
        else
        {
            printb(d_vdev_gic2_dist, "soft interrupt target_list_filter error (=%d)", target_list_filter);
        }
    }
    else
    {
        printb(d_vdev_gic2_dist, "write 0x%X:0x%X error", offset, data);
    }
}

