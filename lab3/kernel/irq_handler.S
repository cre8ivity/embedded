@ irq_handler.S: IRQ Handler
@
@ Authors: ZHUOLIN LIU 1 <zhuolinl>
@          QIAN MAO 2 <qianm>
@          DANLEI ZHOU 3 <danleiz>   
@ Date:    1:04 & 11/2/2013
#include <bits/swi.h>
#include <bits/stack.h>
    .file "irq_handler.S"
    .text

    .global  irq_handler
irq_handler:
    ldr     sp, =IRQ_MODE_STACK @ get irq sp from a fixed place
    stmfd   sp!, {r0-r12, lr}   @ this lr is the original pc

    @ldr     r2, =global_data    @ get r8, currently no use
    @ldr     r8, [r2]

    bl      c_irq_handler       @ execute the irq handler
    ldmfd   sp!, {r0-r12, lr}
    sub     lr, lr, #4          @ next instruction is lr -4
    movs    pc, lr              @ return to original mode
