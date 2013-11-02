@ irq_handler.S: Reentrant SWI Handler
@
@ Authors: ZHUOLIN LIU 1 <zhuolinl>
@          QIAN MAO 2 <qianm>
@          DANLEI ZHOU 3 <danleiz>   
@ Date:    1:04 & 11/2/2013

    .file "irq_handler.S"
    .text

    .global  irq_handler
irq_handler:
    ldr     sp, =irq_sp    
    sub     sp, sp, #4          @leave room on stack for SPSR
    stmfd   sp!, {r0-r12, lr}   @this lr is the user program pc
    mrs     r2, spsr
    str     r2, [sp, #14*4]     @store spsr
    bl      c_irq_handler
    ldr     r2, [sp, #14*4]     @resotre spsr
    msr     spsr, r2
    ldmfd   sp!, {r0-r12, lr}
    add     sp, sp, #4          @remove space used to store SPSR
    movs    pc, lr              @return to user mode