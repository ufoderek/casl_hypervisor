#ifndef _DEV_TIMER_H_
#define _DEV_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum timer_mode
{
    TimerMode_Free,
    TimerMode_Period
} timer_mode;

typedef struct dev_timer
{
    uint32_t base_address;

    uint32_t load[2];
    uint32_t value[2];
    uint32_t control[2];
    uint32_t control_reg[2];
    uint32_t int_clear[2];
} dev_timer;

void dev_timer_init(dev_timer* me, uint32_t base_address);

void dev_timer_set_wrapping(dev_timer* me, int n, bool wrapping);

void dev_timer_set_size(dev_timer* me, int n, int size);

void dev_timer_set_prescale(dev_timer* me, int n, int prescale);

void dev_timer_set_interrupt(dev_timer* me, int n, bool int_en);

void dev_timer_set_mode(dev_timer* me, int n, timer_mode mode);

void dev_timer_set_enable(dev_timer* me, int n, bool en);

uint32_t dev_timer_get_load(dev_timer* me, int n);

void dev_timer_set_load(dev_timer* me, int n, uint32_t load);

uint32_t dev_timer_get_value(dev_timer* me, int n);

void dev_timer_set_value(dev_timer* me, int n, uint32_t value);

void dev_timer_clear_interrupt(dev_timer* me, int n);

#endif
