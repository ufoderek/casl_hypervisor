#include <dev_timer.h>
#include <io.h>
#include <bit_opt_v2l.h>

void dev_timer_init(dev_timer* me, uint32_t base_address)
{
    int i;

    me->base_address = base_address;

    me->load[0] = base_address + 0x0000;
    me->value[0] = base_address + 0x0004;
    me->control[0] = base_address + 0x0008;
    me->control_reg[0] = 0;
    me->int_clear[0] = base_address + 0x000c;

    me->load[1] = base_address + 0x0020;
    me->value[1] = base_address + 0x0024;
    me->control[1] = base_address + 0x0028;
    me->control_reg[1] = 0;
    me->int_clear[1] = base_address + 0x002c;

    for(i = 0; i <= 1; i++)
    {
        dev_timer_set_wrapping(me, i, false);
        dev_timer_set_size(me, i, 32);
        dev_timer_set_prescale(me, i, 1);
        dev_timer_set_interrupt(me, i, false);
        dev_timer_set_mode(me, i, TimerMode_Period);
        dev_timer_set_enable(me, i, false);
    }
}

void dev_timer_set_wrapping(dev_timer* me, int n, bool wrapping)
{
    if(wrapping)
    {
        set_bit(&me->control_reg[n], 0, 0);
    }
    else
    {
        set_bit(&me->control_reg[n], 1, 0);
    }

    io_writew(me->control[n], me->control_reg[n]);
}

void dev_timer_set_size(dev_timer* me, int n, int size)
{
    switch(size)
    {
        case 16:
            set_bit(&me->control_reg[n], 1, 0);
            break;
        case 32:
            set_bit(&me->control_reg[n], 1, 1);
            break;
    }

    io_writew(me->control[n], me->control_reg[n]);
}

void dev_timer_set_prescale(dev_timer* me, int n, int prescale)
{
    switch(prescale)
    {
        case 1:
            set_field(&me->control_reg[n], 3, 2, 0b00);
            break;
        case 16:
            set_field(&me->control_reg[n], 3, 2, 0b01);
            break;
        case 256:
            set_field(&me->control_reg[n], 3, 2, 0b10);
            break;
    }

    io_writew(me->control[n], me->control_reg[n]);
}

void dev_timer_set_interrupt(dev_timer* me, int n, bool int_en)
{
    if(int_en)
    {
        set_bit(&me->control_reg[n], 5, 1);
    }
    else
    {
        set_bit(&me->control_reg[n], 5, 0);
    }

    io_writew(me->control[n], me->control_reg[n]);
}

void dev_timer_set_mode(dev_timer* me, int n, timer_mode mode)
{
    switch(mode)
    {
        case TimerMode_Free:
            set_bit(&me->control_reg[n], 6, 0);
            break;
        case TimerMode_Period:
            set_bit(&me->control_reg[n], 6, 1);
            break;
    }

    io_writew(me->control[n], me->control_reg[n]);
}

void dev_timer_set_enable(dev_timer* me, int n, bool en)
{
    if(en)
    {
        set_bit(&me->control_reg[n], 7, 1);
    }
    else
    {
        set_bit(&me->control_reg[n], 7, 0);
    }

    io_writew(me->control[n], me->control_reg[n]);
}

uint32_t dev_timer_get_load(dev_timer* me, int n)
{
    return io_readw(me->load[n]);
}

void dev_timer_set_load(dev_timer* me, int n, uint32_t load)
{
    io_writew(me->load[n], load);
}

uint32_t dev_timer_get_value(dev_timer* me, int n)
{
    return io_readw(me->value[n]);
}

void dev_timer_set_value(dev_timer* me, int n, uint32_t value)
{
    io_writew(me->value[n], value);
}

void dev_timer_clear_interrupt(dev_timer* me, int n)
{
    io_writew(me->int_clear[n], 0);
}

