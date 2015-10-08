#include <io.h>

uint8_t io_readb(const volatile uint32_t addr)
{
    return *(volatile uint8_t*)addr;
}

uint16_t io_readh(const volatile uint32_t addr)
{
    return *(volatile uint16_t*)addr;
}

uint32_t io_readw(const volatile uint32_t addr)
{
    return *(volatile uint32_t*)addr;
}

uint64_t io_readd(const volatile uint32_t addr)
{
    return (((uint64_t)io_readw(addr + 4)) << 32) | (uint64_t)io_readw(addr);
}

void io_writeb(const volatile uint32_t addr, const uint8_t data)
{
    *(volatile uint8_t*)addr = data;
}

void io_writeh(const volatile uint32_t addr, const uint16_t data)
{
    *(volatile uint16_t*)addr = data;
}

void io_writew(const volatile uint32_t addr, const uint32_t data)
{
    *(volatile uint32_t*)addr = data;
}

void io_writed(const volatile uint32_t addr, const uint64_t data)
{
    io_writew(addr, (uint32_t)data);
    io_writew(addr + 4, (uint32_t)(data >> 32));
}

