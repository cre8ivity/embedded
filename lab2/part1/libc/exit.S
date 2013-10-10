@ exit.S: exit sycall wrapper
@
@ Authors: ZHUOLIN LIU 1 <zhuolinl>
@          QIAN MAO 2 <qianm>
@          DANLEI ZHOU 3 <danleiz> 
@ Date:    21:45 & 10/9/2013

#include <bits/swi.h>

	.file	"exit.S"
	.text

	.global	exit
exit:
	swi	EXIT_SWI
