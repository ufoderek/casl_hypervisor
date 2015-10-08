#include <dev_rom.h>
#include <io.h>

void dev_rom_init(dev_rom* dev_rom, uint32_t base_address)
{
    dev_rom->base_address = base_address;
}

void dev_rom_set_file(dev_rom* dev_rom, uint32_t file_id)
{
    int i;
    uint32_t size;

    dev_rom->cur_address = dev_rom->base_address;

    for(i = 0; i < file_id; i++)
    {
        //skip files
        size = io_readw(dev_rom->cur_address);
        dev_rom->cur_address += (size + 4);
    }
    dev_rom->size = io_readw(dev_rom->cur_address);
    dev_rom->cur_address += 4;
}

uint8_t dev_rom_readb(dev_rom* dev_rom)
{
    return io_readb(dev_rom->cur_address++);
}

uint32_t dev_rom_readw(dev_rom* dev_rom)
{
    uint32_t word = io_readw(dev_rom->cur_address);

    dev_rom->cur_address += 4;

    return word;
}

