@ exit.S: exit system call
@
@ Authors: ZHUOLIN LIU 1 <zhuolinl>
@          QIAN MAO 2 <qianm>
@          DANLEI ZHOU 3 <danleiz>   
@ Date:    1:55 & 10/12/2013
#include <bits/stack.h>

    .file "exit.S"
    .text

    .global exit

@the stack should be lr_svc, r8, spsr, pc_user, r12...r0
exit:
    ldr     r2, =saved_sp_add
    ldr     sp, [r2]             @restore saved sp
    ldr     r3, =saved_kernel_cpsr
    ldr     r2, [r3]
    msr     cpsr, r2             @restore saved cpsr when jump into user program
    ldmfd   sp!, {r4-r12, lr}
    mov     pc, lr               @return to kernel
