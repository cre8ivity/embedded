@ write.S: write sycall wrapper
@
@ Authors: ZHUOLIN LIU 1 <zhuolinl>
@          QIAN MAO 2 <qianm>
@          DANLEI ZHOU 3 <danleiz> 
@ Date:    21:45 & 10/9/2013

#include <bits/swi.h>

    .file   "write.S"
    .text

    .global write
write:
    stmfd   sp!, {r1}
    swi     WRITE_SWI
    cmp     r0, #0          @update errno if error
    bge     end
    ldr     r1, =errno      @read external errno
    rsb     r0, r0, #0
    str     r0, [r1]        @write back errno
    mov     r0, #-1         
end:
    ldmfd   sp!, {r1}    @return
    mov     pc, lr
