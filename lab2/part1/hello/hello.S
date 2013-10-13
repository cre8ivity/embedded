@ hello.S: Hello world! in assembly
@
@ Authors: ZHUOLIN LIU 1 <zhuolinl>
@          QIAN MAO 2 <qianm>
@          DANLEI ZHOU 3 <danleiz>
@ Date:    18:32 & 10/13/2013
#include <bits/swi.h>
#include <bits/fileno.h>

    .file	"hello.S"
    .global	_start
_start:
    mov	r0, #STDOUT_FILENO     @ fd = stdout
    ldr	r1, phello_str
    mov	r2, #hello_size        @ length of "Hello world!\n"
    swi	WRITE_SWI
    mov	r0, #0
    swi	EXIT_SWI
phello_str:
    .word       hello_str
    .section    .rodata
hello_str:
    .ascii      "Hello world!\n"
    .set        hello_size, .-hello_str
