@ hello.S: Hello world! in assembly
@
@ Authors: ZHUOLIN LIU 1 <zhuolinl>
@          QIAN MAO 2 <qianm>
@          DANLEI ZHOU 3 <danleiz>   
@ Date:    21:45 & 10/9/2013
#include <bits/swi.h>
#include <bits/fileno.h>

	.file	"hello.S"
	.section	.rodata
	.align	2
.LC0:
	.ascii	"Hello world!\n"
	.text

	.global	_start
_start:
	mov r0, #STDOUT_FILENO    @ fd = stdout
	ldr r1, .L1
	mov r2, #13    @ the length of "Hello world\n"
	swi WRITE_SWI
	mov	r0, #42
	swi	EXIT_SWI
.L1:
	.word .LC0
