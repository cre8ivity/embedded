@ restore_r8.S: restore r8 when kernel return
@
@ Authors: ZHUOLIN LIU 1 <zhuolinl>
@          QIAN MAO 2 <qianm>
@          DANLEI ZHOU 3 <danleiz>   
@ Date:    1:55 & 10/12/2013
    .file "restore_r8.S"
    .test

    .global restore_r8
restore_r8:
    ldmfd   sp!, {r8}
    mov     pc, lr
