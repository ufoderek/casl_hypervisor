#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
    uint32_t imm = atoi(argv[1]);

    printf("imm = %d\n", imm);

    switch(imm)
    {
        case 1:
            asm volatile("hvc 0x0001");
            break;
        case 2:
            asm volatile("hvc 0x0002");
            break;
        case 3:
            asm volatile("hvc 0x0003");
            break;
        case 4:
            asm volatile("hvc 0x0004");
            break;
        case 5:
            asm volatile("hvc 0x0005");
            break;
        case 6:
            asm volatile("hvc 0x0006");
            break;
        case 7:
            asm volatile("hvc 0x0007");
            break;
        case 8:
            asm volatile("hvc 0x0008");
            break;
        case 9:
            asm volatile("hvc 0x0009");
            break;
        case 10:
            asm volatile("hvc 0x000A");
            break;
        case 0:
        default:
            printf("hvc_helper error\n");
            break;
    }
    return 0;
}

