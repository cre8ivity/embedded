@ exit.S: exit system call
@
@ Authors: ZHUOLIN LIU 1 <zhuolinl>
@          QIAN MAO 2 <qianm>
@          DANLEI ZHOU 3 <danleiz>   
@ Date:    1:55 & 10/12/2013
    .file "exit.S"
    .test

    .global exit

@the stack should be lr_svc, r8, spsr, pc_user, r12...r0
exit:
    add     sp, sp, #15*4    @restore the svc_stack
    ldr     lr, [sp, #4]     @stack has only lr_svc and r8
    mov     pc, lr