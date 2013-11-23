/** @file run_queue.c
 * 
 * @brief Run queue maintainence routines.
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-21
 */

#include <types.h>
#include <assert.h>
#include <exports.h>
#include <kernel.h>
#include <sched.h>
#include "sched_i.h"

#define OSTCBX_MASK 0x07
#define OSTCBY_SHIFT 3

static tcb_t* run_list[OS_MAX_TASKS];

/* A high bit in this bitmap means that the task whose priority is
 * equal to the bit number of the high bit is runnable.
 */
static uint8_t run_bits[OS_MAX_TASKS/8];

/* This is a trie structure.  Tasks are grouped in groups of 8.  If any task
 * in a particular group is runnable, the corresponding group flag is set.
 * Since we can only have 64 possible tasks, a single byte can represent the
 * run bits of all 8 groups.
 */
static uint8_t group_run_bits;

/* This unmap table finds the bit position of the lowest bit in a given byte
 * Useful for doing reverse lookup.
 */
static uint8_t prio_unmap_table[] =
{

0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};


void print_runqueue();
void print_tcb_t(tcb_t* t);

/**
 * @brief Clears the run-queues and sets them all to empty.
 */
void runqueue_init(void)
{
    int i;

	group_run_bits = 0;
    for (i = 0; i < OS_MAX_TASKS/8; i++) {
        run_bits[i] = 0;
    }
}


/**
 * @brief Adds the thread identified by the given TCB to the runqueue at
 * a given priority.
 *
 * The native priority of the thread need not be the specified priority.  The
 * only requirement is that the run queue for that priority is empty.  This
 * function needs to be externally synchronized.
 */
void runqueue_add(tcb_t* tcb, uint8_t prio)
{
	uint8_t oxtcbx, oxtcby;

    oxtcbx = prio & OSTCBX_MASK;
    oxtcby = prio >> OSTCBY_SHIFT;

    // set up the oxtcbx in run bits
    run_bits[oxtcby] |= (1 << oxtcbx);
    // set up the oxtcby in group run bits
    group_run_bits |= (1 << oxtcby);

    // update run list
    //run_list[prio] = &system_tcb[prio];
    run_list[prio] = tcb;
    //printf("----------------\n");
    //printf("in runqueue_add\n\n");
    //printf("prio address: %x, tcb address: %x\n", (unsigned int)&prio, (unsigned int)tcb);
    //print_runqueue();
    //printf("----------------\n");

}


/**
 * @brief Empty the run queue of the given priority.
 *
 * @return  The tcb at enqueued at the given priority.
 *
 * This function needs to be externally synchronized.
 */
tcb_t* runqueue_remove(uint8_t prio)
{
    uint8_t oxtcbx, oxtcby; 
    //tcb_t* return_tcb =  &system_tcb[prio];
    tcb_t* return_tcb = run_list[prio];
    //printf("--------------return_tcb address: %x\n", (unsigned int)return_tcb);


    oxtcbx = prio & OSTCBX_MASK;
    oxtcby = prio >> OSTCBY_SHIFT;

    // clear the bit of oxtcbx
    run_bits[oxtcby] &= (~(1 << oxtcbx));
    // clear the bit of oxtcby if the group is 0
    if (!run_bits[oxtcby]) {
        group_run_bits &= (~(1 << oxtcby));
    }


    // update run list
    run_list[prio] = NULL;
    //printf("----------------\n");
    //printf("in runqueue_remove\n\n");
    //print_runqueue();
    //printf("----------------\n");

    //print_tcb_t(return_tcb);
    //printf("launch_task address: %x\n", (unsigned int)&launch_task);
	return return_tcb;	
}

/**
 * @brief This function examines the run bits and the run queue and returns the
 * priority of the runnable task with the highest priority (lower number).
 */
uint8_t highest_prio(void)
{
	uint8_t oxtcbx, oxtcby;

    oxtcby = prio_unmap_table[group_run_bits];
    oxtcbx = prio_unmap_table[run_bits[oxtcby]];

    return (oxtcby << OSTCBY_SHIFT) + oxtcbx;
}

void print_runqueue()
{   
    int i;

    printf("group_run_bits: %x\n", group_run_bits);
    printf("run_bits:\n");
    for (i = 0; i < OS_MAX_TASKS/8; i++) {
        printf("%x, ", run_bits[i]);
    }
    printf("\n\n");
}

void print_tcb_t(tcb_t* t)
{
    printf("printing tcb_t:\n");
    sched_context_t context = t->context;
    printf("r4: %x, r5: %x, r6: %x\n", context.r4, context.r5, context.r6);
    printf("sp: %x, lr: %x\n\n", (uint32_t)context.sp, (uint32_t)context.lr);
}
