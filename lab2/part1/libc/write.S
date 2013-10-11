@ write.S: write sycall wrapper
@
@ Authors: ZHUOLIN LIU 1 <zhuolinl>
@          QIAN MAO 2 <qianm>
@          DANLEI ZHOU 3 <danleiz> 
@ Date:    21:45 & 10/9/2013

#include <bits/swi.h>

	.file	"write.S"
	.text

	.global	write
write:
	stmfd sp!, {r0-r3}
	swi WRITE_SWI
	mov r2, r0
	bl  __errno_location   @read external errno                                               
	mov r3, r0 
	mov r0, #0
	cmp r2, #0    @update errno if error
	sublt r1, r0, r2
	strlt r1, [r3, #0]    @write back errno
	movlt r0, #-1    @return
	ldmfd sp!, {r0-r3}
	bx	lr
