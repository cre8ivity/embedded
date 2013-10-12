@ exit.S: exit system call
@
@ Authors: ZHUOLIN LIU 1 <zhuolinl>
@          QIAN MAO 2 <qianm>
@          DANLEI ZHOU 3 <danleiz>   
@ Date:    1:55 & 10/12/2013
    .file "exit.S"
    .test

    .global exit
exit:
    ldr     lr, [sp, #13*4]     @lr_svc stores in [sp + 13*4]
    mov     pc, lr
