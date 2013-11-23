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
#include <exports.h> // temp
#ifdef DEBUG_MUTEX

#endif

mutex_t gtMutex[OS_NUM_MUTEX];
// the number of mutexs has been allocated up to now
int allocated_mutex;
void print_mutex_sleep_queue();
void print_tcp(tcb_t* tcb);

void mutex_init()
{
	int i;

    for (i = 0; i < OS_NUM_MUTEX; i++) {
        gtMutex[i].bAvailable = TRUE;
        gtMutex[i].pHolding_Tcb = NULL;
        gtMutex[i].bLock = FALSE;
        gtMutex[i].pSleep_queue = NULL;
    }

    allocated_mutex = 0;
}

int mutex_create(void)
{
    int return_val;

    disable_interrupts();

	if (allocated_mutex >= OS_NUM_MUTEX) {
        puts("No available mutex any more!");
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
    tcb_t *current_tcb, *tmp_tcb;

    disable_interrupts();

    // mutex number out of bound or not allocated
	if (mutex < 0 || mutex >= allocated_mutex) {
        puts("The mutex number is no available!");
        return_val = -EINVAL;

    } else {
        current_tcb = get_cur_tcb();
        target = &gtMutex[mutex];

        // the current task is already holding the lock
        if (target->pHolding_Tcb == current_tcb) {
            puts("The current task is already holding the lock!");
            return_val = -EDEADLOCK;

        } else {
            // can not get the lock, add to sleep queue and 
            // make current task sleep
            if (target->bLock) {
                // add it to sleep queue
                if (!(target->pSleep_queue)) {
                    target->pSleep_queue = current_tcb;
                } else {
                    // add in the tail
                    //print_mutex_sleep_queue();
                    tmp_tcb = target->pSleep_queue;
                    //print_tcp(tmp_tcb);
                    while(tmp_tcb->sleep_queue) {
                        tmp_tcb = tmp_tcb->sleep_queue;
                        //print_tcp(tmp_tcb);
                    }

                    tmp_tcb->sleep_queue = current_tcb;
                    //print_tcp(tmp_tcb);
                    //printf("current_tcb: ");
                    //print_tcp(current_tcb);
                }
                //print_mutex_sleep_queue();
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
    tcb_t *current_tcb, *tmp_tcb;

    disable_interrupts();

    // mutex number out of bound or not allocated
    if (mutex < 0 || mutex >= allocated_mutex) {
        puts("The mutex number is no available!");
        return_val = -EINVAL;

    } else {
        current_tcb = get_cur_tcb();
        target = &gtMutex[mutex];

        // the current task does not hold the lock
        if (target->pHolding_Tcb != current_tcb) {
            puts("The current task does not hold the lock!");
            return_val = -EPERM;

        } else {
            // unlock the mutex
            //print_mutex_sleep_queue();
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
void print_mutex_sleep_queue(){
     mutex_t* target;
     target = &gtMutex[0];
     tcb_t* current = target->pSleep_queue;
        printf("Printing sleep queue for mutex[0]:\n");
        
        while (current) {
            printf("-> %d", (int)current->cur_prio);
            current = current->sleep_queue;
        }
        printf("\n");
}

void print_tcp(tcb_t* tcb) {
    if (!tcb) {
        return;
    } else {
        printf("tcp: %d", (int)tcb->cur_prio);
        if (tcb->sleep_queue) {
            printf(", and next : %d", (int)tcb->sleep_queue->cur_prio);
        }
        printf("\n");
    }
}

