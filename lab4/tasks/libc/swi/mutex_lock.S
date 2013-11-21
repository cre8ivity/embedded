/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>
#include <bits/swi.h>

	.file "mutex_lock.S"

FUNC(mutex_lock)
    swi     MUTEX_LOCK
    cmp     r0, #0          @update errno if error
    bge     end
    ldr     r1, =errno      @read external errno
    rsb     r0, r0, #0
    str     r0, [r1]        @write back errno
    mov     r0, #-1         
end:
    mov     pc, lr          @return
