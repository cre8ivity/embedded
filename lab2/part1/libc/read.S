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
	swi READ_SWI
	mov r2, r0
	bl  __errno_location                                                
    mov r3, r0 
	mov r0, #0
	cmp r2, #0
	sublt r4, r0, r2
	strlt r4, [r3, #0]
	movlt r0, #-1
	bx	lr
