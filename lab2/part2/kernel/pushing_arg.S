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
    stmfd   sp!, {r8}               @store r8

    mrs     r2, cpsr                @load cpsr
    bic     r2, r2, #0xff
    orr     r2, r2, #0xd0           @disable FIQ and IRQ
    msr     spsr, r2
    movs    pc, pc                  @switch to user mode

    ldr     sp, =USER_MODE_STACK    @setup a full descending user mode stack
    stmfd   sp!, {r0-r1}            @push u-boot argc & argv onto stack
    ldr     pc, USER_PRG_ADD        @jump to user program   
