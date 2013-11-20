/**
 * @file device.c
 *
 * @brief Implements simulated devices.
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-12-01
 */

#include <types.h>
#include <assert.h>

#include <task.h>
#include <sched.h>
#include <device.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>

/**
 * @brief Fake device maintainence structure.
 * Since our tasks are periodic, we can represent 
 * tasks with logical devices. 
 * These logical devices should be signalled periodically 
 * so that you can instantiate a new job every time period.
 * Devices are signaled by calling dev_update 
 * on every timer interrupt. In dev_update check if it is 
 * time to create a tasks new job. If so, make the task runnable.
 * There is a wait queue for every device which contains the tcbs of
 * all tasks waiting on the device event to occur.
 */

struct dev
{
	tcb_t* sleep_queue;
	unsigned long   next_match;
};
typedef struct dev dev_t;

/* devices will be periodically signaled at the following frequencies */
const unsigned long dev_freq[NUM_DEVICES] = {100, 200, 500, 50};
static dev_t devices[NUM_DEVICES];

/**
 * @brief Initialize the sleep queues and match values for all devices.
 */
void dev_init(void)
{
    int i;
    for (i = 0; i < NUM_DEVICES; i++) {
        devices[i].next_match = dev_freq[i];
        devices[i].sleep_queue = 0;
    }
}


/**
 * @brief Puts a task to sleep on the sleep queue until the next
 * event is signalled for the device.
 *
 * @param dev  Device number.
 */
void dev_wait(unsigned int dev)
{   
    // get the current task's tcb, no need to handle fault dev
	tcb_t* current_task_tcb = get_cur_tcb();

    // insert the task on the head of device sleep queue
    current_task_tcb->sleep_queue = devices[dev].sleep_queue;
    devices[dev].sleep_queue = current_task_tcb;

    // make it sleep
    dispatch_sleep();
}


/**
 * @brief Signals the occurrence of an event on all applicable devices. 
 * This function should be called on timer interrupts to determine that 
 * the interrupt corresponds to the event frequency of a device. If the 
 * interrupt corresponded to the interrupt frequency of a device, this 
 * function should ensure that the task is made ready to run 
 */
void dev_update(unsigned long millis)
{
    int i;
	for (i = 0; i < NUM_DEVICES; i++) {
        // if a devices task match the time the add all sleep takes to the run queue
        if (devices[i].next_match <= millis) {
            while (devices[i].sleep_queue) {
                tcb_t* head = devices[i].sleep_queue;
                runqueue_add(head, head->cur_prio);
                // go to the next sleep task
                devices[i].sleep_queue = head.sleep_queue;
                // set the current task's next to null
                head.sleep_queue = null;
            }

            // update next match
            devices[i].next_match += dev_freq[i];
        }
    }

    // do context switch after the devices state are updated
    dispatch_save();
}

