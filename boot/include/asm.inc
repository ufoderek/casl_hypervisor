.equ MODE_USR, 0x10
.equ MODE_FIQ, 0x11
.equ MODE_IRQ, 0x12
.equ MODE_SVC, 0x13
.equ MODE_ABT, 0x17
.equ MODE_HYP, 0x1A
.equ MODE_UND, 0x1B
.equ MODE_SYS, 0x1F

.equ STACK_BOTTOM,          128 << 20   @ let VMM use the beginning of the 128 MiB DRAM
.equ STACK_HYP_SIZE,        32 << 20    @ 32 MiB, stack for HYP Mode

.equ STACK_HYP_BASE,        0x06000000  @ 0x04000000 ~ 0x05FFFFFF, stack for hyp mode

.macro mov32 rd, val
        movw    \rd, #(\val) & 0xFFFF
    .if(\val) >> 16
        movt    \rd, #(\val) >> 16
    .endif
.endm

