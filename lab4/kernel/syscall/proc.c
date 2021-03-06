/** @file proc.c
 * 
 * @brief Implementation of `process' syscalls
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-12
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/physmem.h>
#include <device.h>
#include <lock.h>
#include <sched.h>
#include "sched_i.h"
#include <arm/exception.h>


int task_create(task_t* tasks, size_t num_tasks)
{


    disable_interrupts();
    unsigned int i;//, j;
    task_t idle_task_t;
    //task_t tmp;
    int isSchedulable;

    // return if number of task is to large, available task - idle task
    if (num_tasks > OS_AVAIL_TASKS - 1) {
        return -EINVAL;
    }

    // check the task address is in bound
    if (!valid_addr(tasks, sizeof(task_t) * num_tasks, USR_START_ADDR, 
        USR_END_ADDR)) {
        printf("Invalid task address\n");
        return -EFAULT;
    }

    // validate each task
    for (i = 0; i < num_tasks; i++) {
        printf("reason: C:%d, T:%d\n", (int)tasks[i].C, (int)tasks[i].T);
        if (!tasks[i].lambda || !tasks[i].stack_pos
            || tasks[i].C > tasks[i].T) {
            printf("Invalid task %d\n", i);
            return -EFAULT;
        }
    }

    // ub_test
    isSchedulable = assign_schedule(&tasks, num_tasks);
    if (isSchedulable) {
        printf("UB_TEST: SUCCESS!\n");
    } else {
        printf("UB_TEST: UNSUCCESS!\n");
    }

    // initial device, mutex and runqueue
    dev_init();
    // initiating mutex is moved to kmain
    runqueue_init();

    //sort tasks, high frequency task has higher priority
    //actually we don't need to do this after ub-test,
    //leave here for the convenience of testing part1
    /*for (i = 0; i < num_tasks; i++) {
        for (j = i+1; j < num_tasks; j++) {
            // bubble sort, swap
            if (tasks[j].T < tasks[i].T) {
                tmp = tasks[j];
                tasks[j] = tasks[i];
                tasks[i] = tmp;
            }
        }
    }*/

    // allocate the task and put into running queue
    allocate_tasks(&tasks, num_tasks);


    // set up the tcb for idle task and put into running queue
    sched_init(&idle_task_t);


    // make the idle task schedulable
    dispatch_init(&system_tcb[IDLE_PRIO]);

    dispatch_nosave();


    return 1;
}

int event_wait(unsigned int dev)
{
    if (dev >= NUM_DEVICES) {
        printf("%uth dev is out of the device range\n", dev);
        return -EINVAL;
    }
    // wait for device running
    if(dev_wait(dev) != 0) {
        return -1;
    }

    return 0;
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num)
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);

	disable_interrupts();
	while(1);
}
