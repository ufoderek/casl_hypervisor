# we put this code at 0x0
.include "asm.inc"

.extern reset_handler
.extern und_handler
.extern svc_handler
.extern pabort_handler
.extern dabort_handler
.extern hyp_trap_handler
.extern irq_handler
.extern fiq_handler

.text     
    @branch within 16 MiB, if too far use "ldr pc, addr" instead
    b reset_handler_init
    b und_handler_init
    b svc_handler_init
    b pabort_handler_init
    b dabort_handler_init
    b hyp_trap_handler_init
    b irq_handler_init
    b fiq_handler_init

/*
.macro handler_template NEW_MODE, NEW_STACK, C_HANDLER
    cpsid aif, \NEW_MODE        @change mode
    mov32 sp, \NEW_STACK        @initialize sp
    push {r0-r12,lr}            @backup regs from guest os, so now we can use all registers
    mov32 r0, \NEW_STACK        @r0 is handler's first parameter, which stores the stack address
    bl \C_HANDLER               @call the handler (a C function), noted that this function may not return
    pop {r0-r12,lr}             @restore regs
    eret                        @return to guest os
.endm
*/

/*
stack diagram:

    R0 | SP-(4x14)
    R1 | SP-(4x13)
    .. | ...
   R10 | SP-(4x4)
   R11 | SP-(4x3) 
   R12 | SP-(4x2)
LR_usr | SP-(4x1)

*/

.macro handler_template C_HANDLER
    cpsid aif, MODE_HYP         @change to HYP mode
    mov32 sp, STACK_HYP_BASE    @initialize sp

    push {r0-r12,lr}            @backup regs from guest os, so now we can use all registers
    mov32 r0, STACK_HYP_BASE    @r0 is handler's first parameter, which stores the stack address
    bl \C_HANDLER               @call the handler (a C function), noted that this function may not return
    pop {r0-r12,lr}

    eret                        @return to guest os
.endm

reset_handler_init:
    cpsid aif, MODE_HYP         @change to HYP mode
    mov32 sp, STACK_HYP_BASE    @initialize sp
    b main                      @never return
    
und_handler_init:
    handler_template und_handler

svc_handler_init:
    handler_template svc_handler

pabort_handler_init:
    handler_template pabort_handler

dabort_handler_init:
    handler_template dabort_handler

hyp_trap_handler_init:
    handler_template hyp_trap_handler

irq_handler_init:
    handler_template irq_handler

fiq_handler_init:
    handler_template fiq_handler

.end

