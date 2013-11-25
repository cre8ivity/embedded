/** @file sched.c
 * 
 * @brief Top level implementation of the scheduler.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

#include <types.h>
#include <assert.h>

#include <kernel.h>
#include <config.h>
#include "sched_i.h"

#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/physmem.h>
#include <arm/exception.h>

tcb_t system_tcb[OS_MAX_TASKS]; /*allocate memory for system TCBs */

static void idle(void);

/**
 * Set up the context of a task and add to running queue
 */
void context_create(task_t* task, size_t priority) {
    //set up the context
    system_tcb[priority].native_prio = priority;
    system_tcb[priority].cur_prio = priority;

    system_tcb[priority].context.r4 = (uint32_t)task->lambda;
    system_tcb[priority].context.r5 = (uint32_t)task->data;
    system_tcb[priority].context.r6 = (uint32_t)task->stack_pos;
    system_tcb[priority].context.sp = (void*)system_tcb[priority].kstack_high;

    // lauch this task when first run
    system_tcb[priority].context.lr = launch_task;

    system_tcb[priority].holds_lock = 0;
    system_tcb[priority].sleep_queue = NULL;


    // add to running queue
    runqueue_add(&system_tcb[priority], priority);
}

/**
 * schedule the idle task when task create
 */
void sched_init(task_t* main_task)
{
    // create the idle task
    main_task->lambda = (task_fun_t)idle;
    main_task->data = NULL;
    main_task->stack_pos = system_tcb[IDLE_PRIO].kstack_high;
    main_task->C = 0;
    main_task->T = 0;

    // create context and add to running queue
    context_create(main_task, IDLE_PRIO);
}

/**
 * @brief This is the idle task that the system runs when no other task is runnable
 */
 
static void idle(void)
{
	 enable_interrupts();
	 while(1);
}

/**
 * @brief Allocate user-stacks and initializes the kernel contexts of the
 * given threads.
 *
 * This function assumes that:
 * - num_tasks < number of tasks allowed on the system.
 * - the tasks have already been deemed schedulable and have been appropriately
 *   scheduled.  In particular, this means that the task list is sorted in order
 *   of priority -- higher priority tasks come first.
 *
 * @param tasks  A list of scheduled task descriptors.
 * @param size   The number of tasks is the list.
 */
void allocate_tasks(task_t** tasks, size_t num_tasks)
{
    size_t i;
    // set up tasks context and put them into runqueue
    for (i = 0; i < num_tasks; i++) {
        context_create(&((*tasks)[i]), i+1);
    }
}

