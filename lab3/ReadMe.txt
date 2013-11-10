/*
 * ReadMe: the document of design, implementation and test
 *
 * Authors: ZHUOLIN LIU 1 <zhuolinl>
 *          QIAN MAO 2 <qianm>
 *          DANLEI ZHOU 3 <danleiz> 
 * Date:    15:10 & 10/31/2013
 */

--------------------------------------------------------------------------------
Part 1: Design & Implementation
--------------------------------------------------------------------------------

1 IRQ Handler
The task of IRQ handler is to 
(1) Change sp to irq_sp;
In our design which is the mid point between svc_stack_base and kernel code. 
We choose this point since we assume our kernel code and svc_stack will not be 
larger than 7,613 MB. In addition, irq_stack will only store the registers when 
IRQ interrupt happens. Because we don't allow nested IRQ interrupts, irq_stack 
will never exceed 56 bytes. We can safely assume in this case, we will never 
corrupt any kernel code or svc_stack.

(2) Keep and restore registers when enter and exit the handler; 
Since IRQ interrupt can happen any time. We should restore the state when the 
interrupt happens. Thus we need to keep and restore all registers [r0-r12, lr].  

(3) Check if the interrupt source is the Timer;
We use a C version IRQ handler to judge and handle IRQ interrupt. In this lab,
we only need to handle timer interrupt, but we need to write a general handler
for future expansion. Therefore, in C_IRQ_Handler, we check if the interrupt 
source is the timer. Otherwise, ignore and return.

(4) If it is Timer, do some operations;
As we keep a global system time, the job of IRQ handler is very simple. It only 
update the global system time, clears OSSR register and set a new match register
which is 10 ms ahead.  

(5) Return to the proper position where the interrupt happened.
One thing we should take care of is the location we should return. Because when
an IRQ interrupt happens, current instruction has not been operated, we should 
manually sub 4 for lr and return to the interrupted instruction again. 


2 System Time
In our design, we set a global system time and consistently update it in every
10 ms. This design significantly simplify the implementation of time() and 
sleep():
(1) time()
If we keep a global system time, the time() syscall only needs to read system 
time and return it to user program.

(2) sleep()
In sleep syscall, a while loop will keep checking if the current system time 
pass some point we set for the wake up. If it is time for waking up, return to 
user program.


--------------------------------------------------------------------------------
Part 2: Test
--------------------------------------------------------------------------------

(1) Correctness
We have run hello, rot13, splat, typo and our fun programs to test our kernel. 
GBD test shows our kernel keeps and restore all necessary registers correctly.

(2) System Time Error (accuracy)
One thing about system time we concerns most is the accuracy. The error comes
from the running time of handler. Initially, we updated OSTMR_OSMR_ADDR(0) by 
adding 10 ms to it. Then we noticed that the error would increase by the time 
and finally miss the next IRQ interrupt (it will wait around 10 min to get next
IRQ interrupt). Therefore, we change to use OSTMR_OSCR_ADDR plus 10 ms as new
match. This solves the missing interrupt problem (it takes much less time than 
10 ms to run IRQ handler). In addition, the error can be corrected every time we
set match register since we always use OSTMR_OSCR_ADDR as the base.

(3) Corner Case
--time():
	In theory, time should always succeed because it only retrieve global system
	time and return that to user program. The only thing we need to take care is
	we should set global time to be volatile since it will be modified by irq
	handler.
--sleep():
	One thing need to be considered is the bound of the system time. In our 
	implementation, we use unsigned long long type to represent system time, 
	which is 64 bits. The system time will increase by 1 every 10 ms. Therefore,
	it can represent up to 5.8 * 10^9 years. In this case, we can safely assume
	the system time will never over flow. By the way, sleep() only accepts 
	non-negative parameter. 

(4) Fun Tests
We have implemented three games for tests. Please refer to:
lab3-Testing Program Implementation.docx 
