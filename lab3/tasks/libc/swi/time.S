/** @file time.S
 *
 * @brief time sycall wrapper
 *
 * @author KHUOLIN LIU 1 <zhuolinl>
 * @          QIAN MAO 2 <qianm>
 * @          DANLEI ZHOU 3 <danleiz>   
 * @ Date:    1:44 & 11/2/2013
 */

#include <asm.h>

	.file "time.S"

FUNC(time)
	@ Add code here.  Put your name at the top.
    swi     TIME_SWI
    mov     pc, lr          @ return
