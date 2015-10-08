#include <idle_thread.h>

void idle_thread()
{
    asm volatile("cpsie aif");

    while(1)
    {
    }
}

