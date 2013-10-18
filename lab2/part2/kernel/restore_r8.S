@ restore_r8.S: restore r8 when kernel return
@
@ Authors: ZHUOLIN LIU 1 <zhuolinl>
@          QIAN MAO 2 <qianm>
@          DANLEI ZHOU 3 <danleiz>   
@ Date:    1:55 & 10/12/2013
    .file "restore_r8.S"
    .text

    .global restore_r8

@the stack should be lr_svc, r8
restore_r8:
    ldmfd   sp!, {r8, lr}
    mov     pc, lr
