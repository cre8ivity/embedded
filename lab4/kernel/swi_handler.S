@ swi_handler.S: Reentrant SWI Handler
@ see sides in Week 6
@
@ Authors: ZHUOLIN LIU 1 <zhuolinl>
@          QIAN MAO 2 <qianm>
@          DANLEI ZHOU 3 <danleiz>   
@ Date:    17:05 & 10/11/2013

    .file "swi_handler.S"
    .text

    .global swi_handler
swi_handler:
    stmfd   sp, {sp, lr}^
    sub     sp, sp, #8


    sub     sp, sp, #4          @leave room on stack for SPSR
    stmfd   sp!, {r0-r12, lr}   @this lr is the user program pc
    
    mrs     r2, cpsr            @enable irq
    bic     r2, #0x80
    msr     cpsr, r2

    mrs     r2, spsr
    str     r2, [sp, #14*4]     @store spsr
    ldr     r1, =global_data
    ldr     r8, [r1]
    @ldr     r8, [sp, #21*4]     @restore r8 for u-boot syscall
    mov     r1, sp
    ldr     r0, [lr, #-4]       @extract the SWI number
    bic     r0, r0, #0xff000000 @mask swi
    bl      c_swi_handler
    ldr     r2, [sp, #14*4]     @resotre spsr
    msr     spsr, r2
    ldmfd   sp!, {r0-r12, lr}
    add     sp, sp, #4          @remove space used to store SPSR


    ldmfd   sp, {sp,lr}^
    add     sp, sp, #8 

    movs    pc, lr              @return to user mode
