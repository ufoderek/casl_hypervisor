#ifndef _DEV_ROM_H_
#define _DEV_ROM_H_

#include <stdint.h>

typedef struct dev_rom
{
    uint32_t base_address;
    uint32_t cur_address;
    uint32_t size;
} dev_rom;

void dev_rom_init(dev_rom* dev_rom, uint32_t base_address);

void dev_rom_set_file(dev_rom* dev_rom, uint32_t file_id);

uint8_t dev_rom_readb(dev_rom* dev_rom);

uint32_t dev_rom_readw(dev_rom* dev_rom);

#endif
