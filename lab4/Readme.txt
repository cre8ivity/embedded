/*
 * ReadMe: the document of design, implementation and test
 *
 * Authors: ZHUOLIN LIU 1 <zhuolinl>
 *          QIAN MAO 2 <qianm>
 *          DANLEI ZHOU 3 <danleiz> 
 * Date:    15:10 & 10/31/2013
 */
--------------------------------------------------------------------------------
Part 1: Overview
--------------------------------------------------------------------------------

In this lab we improved our kernel with context switching, task management and concurrency control.
Also, we designed a UB admission control algorithm to verify the schedulability of the given task set.


--------------------------------------------------------------------------------
Part 2: Design & Implementation
--------------------------------------------------------------------------------

1 Kernel(main.c)

We installed our customed swi and irq handler. Then we initialized timer and mutex. Before we started to excuate user program, we add a loop to  crt0.S to keep user program running.

2 Device(device.c)

In device.c, we implemented dev_init to initialize each device's sleep queue and frequence. Then we implemented 

When a timer sends a interrupt, dev_update will be called to determine whether a device needs to be updated. If updated is needed, it will put all tasks in sleep queue to running queue.

event_wait will call dev_wait to put all tasks in that device to sleep_queue.


3 proc.c
        

task_create function will check if there is any error in each task and then initiate device and runqueue. After that it will put all valid tasks into running queue. Then it will set up the tcb for idle task and put it into running queue .

event_wait function checks if device number is valid and then calls dev_wait function to do the context switch.

4 runqueue.c

We have four functions in this file, runqueue_init, runqueue_add, runqueue_remove and highest_prio.

runqueue_init initializes group_run_bits and run_bits.

runqueue_add adds a tcb into a runqueue and  runqueue_remove returns a tcb according to the given priority num and remove it from runqueue.

As for finding the highest priority in the runqueue, we used prio_unmap_table to do the quick search.

5 context switch(ctx_switch.c)

We have four functions in this file, dispatch_init, dispatch_save, dispatch_nosave, dispatch_sleep.

We used a static variable to store the current tcb.
In dispatch_init, we set current tcb to idle. 
In dispatch_save, if the highest priority task doesn't have the highest priority task in runqueue, we will change current tcb to the highest priority task and then do context switch between  old task and current task.

dispatch_nosave basically do the same thing with dispatch_save except not saving current task's status.

dispatch_sleep will context switch to the highest priority task that is not this task and save the current task but don't put it in running queue.

6 schedule(sched.c)

We have four functions in this file, context_create, sched_init and allocate_tasks.

context_create will set up the context of a task and add it to runqueue.

sched_init will create the idle task and call context_create to create its context.

allocate_tasks sets up given tasks' context and pit them into runqueue. 

7 ub_test(ub_test.c)
We embedded ub_test into create_task to check if the submitted tasks are schedualable. The job of ub_test:
- Sort the tasks by frequency.
- UB test: to boost the test phase, we use a look-up table to store values for 64 tasks for quick check.

8 highest locker protocol
To prevent deadlock, we adopted highest locker protocol. Since we cannot do offline analysis, to simplify the implementation, we will temporily raise the priority of the task calling lock() to 0, and forbid it to call event_wait. After the tasks releases all lockers, its priority will decreases to its origin.
