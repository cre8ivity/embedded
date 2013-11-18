/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-10-31
 */

#include <asm.h>

	.file "time.S"

FUNC(time)
    @ time wrapper, no need to handle return value
    swi     TIME_SWI
    mov     pc, lr          @ return
