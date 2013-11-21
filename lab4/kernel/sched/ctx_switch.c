/** @file ctx_switch.c
 * 
 * @brief C wrappers around assembly context switch routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 */
 

#include <types.h>
#include <assert.h>

#include <config.h>
#include <kernel.h>
#include "sched_i.h"

#ifdef DEBUG_MUTEX
#include <exports.h>
#endif

static tcb_t* cur_tcb;

/**
 * @brief Initialize the current TCB and priority.
 *
 * Set the initialization thread's priority to IDLE so that anything
 * will preempt it when dispatching the first task.
 */
void dispatch_init(tcb_t* idle)
{
	cur_tcb = idle;
    ctx_switch_half(&(idle->context));
}


/**
 * @brief Context switch to the highest priority task while saving off the 
 * current task state.
 *
 * This function needs to be externally synchronized.
 * We could be switching from the idle task.  The priority searcher has been tuned
 * to return IDLE_PRIO for a completely empty run_queue case.
 */
void dispatch_save(void)
{
    uint8_t highest_task_prio;
    tcb_t *next_task_tcb, *tmp_cur_tcb;

    // get the highest_task priority
    highest_task_prio = highest_prio();

    // switch context only when current running task is not the highest prio-task
    if (highest_task_prio != cur_tcb->cur_prio) {
        runqueue_add(cur_tcb, cur_tcb->cur_prio);
        next_task_tcb = runqueue_remove(highest_task_prio);

        // change current tcb and do context switch
        tmp_cur_tcb = cur_tcb;
        cur_tcb = next_task_tcb;

        ctx_switch_full(&(cur_tcb->context), &(tmp_cur_tcb->context));
    }
}

/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * don't save the current task state.
 *
 * There is always an idle task to switch to.
 */
void dispatch_nosave(void)
{   
    // get the highest priority task and directly switch to it
    uint8_t highest_task_prio = highest_prio();

    // switch context only when current running task is not the highest prio-task
    // in case there is only idle task running
    if (highest_task_prio != cur_tcb->cur_prio) {
        cur_tcb = runqueue_remove(highest_task_prio);
        ctx_switch_half(&(cur_tcb->context));
    }
}


/**
 * @brief Context switch to the highest priority task that is not this task -- 
 * and save the current task but don't mark is runnable.
 *
 * There is always an idle task to switch to.
 */
void dispatch_sleep(void)
{
	uint8_t highest_task_prio;
    tcb_t *next_task_tcb, *tmp_cur_tcb;

    // get the highest_task priority
    highest_task_prio = highest_prio();

    // switch context only when current running task is not the highest prio-task
    // do not add the current task into running queue
    if (highest_task_prio != cur_tcb->cur_prio) {
        next_task_tcb = runqueue_remove(highest_task_prio);

        // change current tcb and do context switch
        tmp_cur_tcb = cur_tcb;
        cur_tcb = next_task_tcb;

        ctx_switch_full(&(cur_tcb->context), &(tmp_cur_tcb->context));
    }
}

/**
 * @brief Returns the priority value of the current task.
 */
uint8_t get_cur_prio(void)
{
	return cur_tcb->cur_prio;
}

/**
 * @brief Returns the TCB of the current task.
 */
tcb_t* get_cur_tcb(void)
{
	return cur_tcb;
}
