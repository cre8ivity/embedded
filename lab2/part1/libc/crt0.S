@ crt0.S: Execution startup routine
@
@ Authors: ZHUOLIN LIU 1 <zhuolinl>
@          QIAN MAO 2 <qianm>
@          DANLEI ZHOU 3 <danleiz> 
@ Date:    21:45 & 10/9/2013

    .file   "crt0.S"
    .text

    .global _start
_start:
    ldr     r0, [sp]        @load the argc
    add     r1, sp, #4      @argv* is in the sp + 4
    bl      main
    bl      exit            @call main and exit

