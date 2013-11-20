/**
 * @file mutex.c
 *
 * @brief Implements mutices.
 *
 * @author Harry Q Bovik < PUT YOUR NAMES HERE
 *
 * 
 * @date  
 */

//#define DEBUG_MUTEX

#include <lock.h>
#include <task.h>
#include <sched.h>
#include <bits/errno.h>
#include <arm/psr.h>
#include <arm/exception.h>
#ifdef DEBUG_MUTEX
#include <exports.h> // temp
#endif

mutex_t gtMutex[OS_NUM_MUTEX];
// the number of mutexs has been allocated up to now
int allocated_mutex;

void mutex_init()
{
	int i;

    for (i = 0; i < OS_NUM_MUTEX; i++) {
        gtMutex[i].bAvailable = TRUE;
        gtMutex[i].pHolding_Tcb = NULL;
        gtMutex[i].block = FALSE;
        gtMutex[i].pSleep_queue = NULL;
    }

    allocated_mutex = 0;
}

int mutex_create(void)
{
    int return_val;

    disable_interrupts();

	if (allocated_mutex >= OS_NUM_MUTEX) {
        printf("No available mutex any more!");
        return_val = -ENOMEM;
    } else {
    // have available mutex in the pool
        return_val = allocated_mutex++;
        gtMutex[return_val].bAvailable = FALSE;
    }

    enable_interrupts();
	return return_val;
}

int mutex_lock(int mutex)
{
    int return_val = 0;
    mutex_t* target;
    tcb_t* current_tcb, tmp_tcb;

    disable_interrupts();

    // mutex number out of bound or not allocated
	if (mutex < 0 || mutex >= allocated_mutex) {
        printf("The mutex number is no available!");
        return_val = -EINVAL;

    } else {
        current_tcb = get_cur_tcb();
        target = &gtMutex[mutex];

        // the current task is already holding the lock
        if (target->pHolding_Tcb == current_tcb) {
            printf("The current task is already holding the lock!");
            return_val = -EDEADLOCK;

        } else {
            // can not get the lock, add to sleep queue and 
            // make current task sleep
            if (target->block) {
                // add it to sleep queue
                if (!target->pSleep_queue) {
                    target->pSleep_queue = current_tcb;
                } else {
                    // add in the tail
                    tmp_tcb = target->pSleep_queue;
                    while(!tmp_tcb->sleep_queue) {
                        tmp_tcb = tmp_tcb->sleep_queue;
                    }

                    tmp_tcb->sleep_queue = current_tcb;
                }

                dispatch_sleep();
            }

            // now, the current task can get the lock
            target->pHolding_Tcb = current_tcb;
            target->bLock = TRUE;
        }

    }

    enable_interrupts();
    return return_val;
}

int mutex_unlock(int mutex)
{
	int return_val = 0;
    mutex_t* target;
    tcb_t* current_tcb, tmp_tcb;

    disable_interrupts();

    // mutex number out of bound or not allocated
    if (mutex < 0 || mutex >= allocated_mutex) {
        printf("The mutex number is no available!");
        return_val = -EINVAL;

    } else {
        current_tcb = get_cur_tcb();
        target = &gtMutex[mutex];

        // the current task does not hold the lock
        if (target->pHolding_Tcb != current_tcb) {
            printf("The current task does not hold the lock!");
            return_val = -EPERM;

        } else {
            // unlock the mutex
            target->pHolding_Tcb = NULL;
            target->bLock = FALSE;
            
            // awake the first member in sleep queue
            if (target->pSleep_queue != NULL) {
                // move the head of the linked list to the next
                tmp_tcb = target->pSleep_queue;
                target->pSleep_queue = tmp_tcb->sleep_queue;
                tmp_tcb->sleep_queue = NULL;

                // awake the task
                runqueue_add(tmp_tcb, tmp_tcb->cur_prio);
            }
        }
    }

    enable_interrupts();
    return return_val;
}

