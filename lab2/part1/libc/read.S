@ read.S: read sycall wrapper
@
@ Authors: ZHUOLIN LIU 1 <zhuolinl>
@          QIAN MAO 2 <qianm>
@          DANLEI ZHOU 3 <danleiz> 
@ Date:    21:45 & 10/9/2013

#include <bits/swi.h>

	.file	"read.S"
	.text

	.global	read
read:
	stmfd	sp!, {r0-r2}
	swi		READ_SWI
	cmp		r0, #0			@update errno if error
	bge		end
	ldr		r1, =errno		@read external errno                                               
	mov		r2, #0
	sub		r0, r2, r0
	str 	r0, [r1]		@write back errno
	mov 	r0, #-1			
end:
	ldmfd 	sp!, {r0-r2}	@return
	mov		pc, lr
