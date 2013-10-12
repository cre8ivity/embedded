@ spushing_arg.S: Pushing U-Boot argc & argv on the User Stack
@
@ Authors: ZHUOLIN LIU 1 <zhuolinl>
@          QIAN MAO 2 <qianm>
@          DANLEI ZHOU 3 <danleiz>   
@ Date:    1:05 & 10/12/2013
#include <bits/stack.h>
    .file "pushing_arg.S"
    .test

    .global pushing_arg
pushing_arg:
    mrs     r2, cpsr                @load cpsr, cannot use r0,r1
    bic     r2, r2, #0xff
    orr     r2, r2, #0xc0           @disable FIQ and IRQ
    msr     spsr, r2
    stmfd   sp!, {r8, lr}           @store r8
    mov     lr, pc                  @switch to user mode without change pc
    movs    pc, lr

    ldr     sp, =USER_MODE_STACK    @sp change to sp_user, load large const
    stmfd   sp!, {r0-r1}            @push u-boot argc & argv onto stack
    ldr     pc, =USER_PRG_ADD       @jump to user program   
