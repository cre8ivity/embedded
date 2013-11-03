/** @file sleep.S
 *
 * @brief sleep sycall wrapper
 *
 * @author    ZHUOLIN LIU 1 <zhuolinl>
 * @          QIAN MAO 2 <qianm>
 * @          DANLEI ZHOU 3 <danleiz>   
 * @ Date:    1:41 & 11/2/2013
 */

#include <asm.h>
#include <bits/swi.h>

	.file "sleep.S"

FUNC(sleep)
	@ Add code here.  Put your names at the top.
    swi     SLEEP_SWI
    mov     pc, lr          @ return
