/** @file read.S
 *
 * @brief read sycall wrapper
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 07 Oct 2007 01:53:16 -0400
 * 
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-29
 */

#include <asm.h>
#include <bits/swi.h>

	.file "read.S"

FUNC(read)
    swi     READ_SWI
    cmp     r0, #0          @update errno if error
    bge     end
    ldr     r1, =errno      @read external errno
    rsb     r0, r0, #0
    str     r0, [r1]        @write back errno
    mov     r0, #-1         
end:
    mov     pc, lr          @ return
