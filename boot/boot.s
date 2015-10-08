.include "asm.inc"

/* importing external data */
.extern main

/* export global labels and variables */
.global reset_handler

/* text(code) area directive */
.code 32
.text
.section .start, "x"	@ executable section
.align 4

/* the code entry point */
reset_handler:
    mov r3, r0 /* r0 is sp saved from reset_handler, now use r3 pass to main */ 
	bl main
    /*swi 0xFFFFFF*/

/* the linker parameters and static local data */
start_text:
	.word _start_text		@ start address of text(code) section

length_text:
	.word _length_text		@ length of text(code) section

start_rodata:
	.word _start_rodata		@ start address of read only data section

length_rodata:
	.word _length_rodata	@ length of read only data section

start_data:
	.word _start_data		@ start address of data section

length_data:
	.word _length_data		@ length of data section

start_bss:
	.word _start_bss		@ start address of BSS section

length_bss:
	.word _length_bss		@ length of BSS section

/* end of boot code */
.size reset_handler, . - reset_handler
.end

