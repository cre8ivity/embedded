	.file	"part1-strTable.c"
	.text
	.align	2
	.global	strTable
	.type	strTable, %function
strTable:
	@ Danlei Zhou(danleiz)
	@ Zhuolin Liu(zhuolinl)
	@ Qian Mao(qianm)
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ r0 = src. r1 = dst, r2 = srcLen, r3 = dstLen
	stmfd	sp!, {r5, r6}	@ keep the state of r5 and r6
	mov	r5, r0
	subs r3, r3, #1		@ count down r3 until 0
	blt	.L11
.L9:
	ldrb	r6, [r1, r3]	@ load iteratively from dst
	mov ip, r6
.L7:
	@ do (ip % 23):
	@ first sub 23 then check if the remaining is > 45
	@ if > 45, then sub 46, loop back if the remaining larger than 22
	sub	ip, ip, #23	
	cmp	ip, #45		
	subgt ip, ip, #46
	cmp ip, #22
	bgt	.L7
	cmp	ip, r2		@ check if ip out of bound of src
	bgt	.L4
	ldrb	r0, [r5, ip]	@ replace src char if dst char is larger than src
	cmp	r0, r6  
	strneb	r6, [r5, ip]
.L4:
	subs r3, r3, #1  
	bge	.L9
.L11:
	ldmfd	sp!, {r5, r6}	@ recover r5 and r6
	mov pc, lr
	.size	strTable, .-strTable
	.ident	"GCC: (GNU) 3.4.5"
