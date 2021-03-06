@ spushing_arg.S: Pushing U-Boot argc & argv on the User Stack
@
@ Authors: ZHUOLIN LIU 1 <zhuolinl>
@          QIAN MAO 2 <qianm>
@          DANLEI ZHOU 3 <danleiz>   
@ Date:    1:05 & 10/12/2013
#include <bits/stack.h>
#include <bits/swi.h>
    .file "exec.S"
    .text

    .global exec
exec:
    stmfd   sp!, {r4-r12, lr}       @APCS convention
    ldr     r2, =saved_sp_add
    str     sp, [r2]                @store svc_sp
    mrs     r2, cpsr                @load cpsr, cannot use r0,r1
    bic     r2, r2, #0xff
    orr     r2, r2, #0xd0           @disable FIQ and IRQ, and change mode
    msr     cpsr, r2                @switch to user mode
    
    ldr     sp, =USER_MODE_STACK    @sp change to sp_user, load large const
    mov     r2, #0                  
    str     r2, [sp], #-4           @pushing first NULL into user stack
    mov     r3, r0                  @r3 = argc
loop:
    subs    r3, r3, #1
    blt     end
    ldr     r2, [r1, r3, LSL #2]
    str     r2, [sp], #-4           @pushing argv[i] into [sp + 4*i + 4]
    b       loop
end:
    str     r0, [sp]                @pushing argc
    mov     lr, pc                  @function as BL
    ldr     pc, =USER_PRG_ADD       @jump to user program
    swi     EXIT_SWI                @exit to kernel
