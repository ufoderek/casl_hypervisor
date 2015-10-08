#include <sys/stat.h>

#include <debug_utils.h>
#include <dev_semihost.h>
#include <dev_uart.h>

dev_uart* newlib_uart;
dev_semihost* newlib_semihost;

void _exit(int status)
{
    asm volatile("swi #0xFFFFFF");

    while(1)
    {
        ;    //shut up gcc
    }
}

int _close(int file)
{
    return dev_semihost_do_close(newlib_semihost, file);
}

int _fstat(int file, struct stat* st)
{
    return dev_semihost_do_fstat(newlib_semihost, file, st);
}

int _isatty(int file)
{
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    return dev_semihost_do_lseek(newlib_semihost, file, ptr, dir);
}

int _open(const char* name, int flags, int mode)
{
    return dev_semihost_do_open(newlib_semihost, name, flags, mode);
}

int _read(int file, char* ptr, int len)
{
    return dev_semihost_do_read(newlib_semihost, file, ptr, len);
}

caddr_t _sbrk(int incr)
{
    //newlib will ask for continuous memory chunk

    //max range of heap
    const uint32_t heap_start = 0x02000000;
    const uint32_t heap_end = 0x04000000;

    //current range of heap
    static uint32_t current_heap_end = heap_start;

    uint32_t prev_heap_end;

    prev_heap_end = current_heap_end;

    //check if out of memory
    if(current_heap_end + incr > heap_end)
    {
        return (caddr_t)0;
    }

    current_heap_end += incr;

    return (caddr_t)prev_heap_end;
}

int _write(int file, char* ptr, int len)
{
    int i = 0;
    if((file == (int)stdout) || (file == (int)stderr))
    {
        for(i = 0; i < len; i++)
        {
            if(ptr[i] != '\r')
            {
                dev_uart_tx_char(newlib_uart, ptr[i]);
            }
        }
        return len;
    }
    else
    {
        return dev_semihost_do_write(newlib_semihost, file, ptr, len);
    }
}
