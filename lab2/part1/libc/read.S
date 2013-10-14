@ read.S: read sycall wrapper
@
@ Authors: ZHUOLIN LIU 1 <zhuolinl>
@          QIAN MAO 2 <qianm>
@          DANLEI ZHOU 3 <danleiz> 
@ Date:    21:45 & 10/9/2013

#include <bits/swi.h>

    .file   "read.S"
    .text

    .global read
read:
    swi     READ_SWI
    cmp     r0, #0          @update errno if error
    bge     end
    ldr     r1, =errno      @read external errno
    rsb     r0, r0, #0
    str     r0, [r1]        @write back errno
    mov     r0, #-1         
end:
    mov     pc, lr          @ return
