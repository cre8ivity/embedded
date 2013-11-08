/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author    ZHUOLIN LIU 1 <zhuolinl>
 * @          QIAN MAO 2 <qianm>
 * @          DANLEI ZHOU 3 <danleiz>   
 * @ Date:    1:44 & 11/2/2013
 */

#include <asm.h>
#include <bits/swi.h>

	.file "time.S"

FUNC(time)
	@ time wrapper, no need to handle return value
    swi     TIME_SWI
    mov     pc, lr          @ return
