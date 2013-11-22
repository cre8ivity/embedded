/*
 * c_swi_handler.c: swi dispatcher
 *
 * Authors: ZHUOLIN LIU 1 <zhuolinl>
 *          QIAN MAO 2 <qianm>
 *          DANLEI ZHOU 3 <danleiz> 
 * Date:    17:45 & 10/11/2013
 */
#include <types.h>
#include <bits/fileno.h>
#include <bits/swi.h>
#include <bits/stack.h>
#include <exports.h>
#include <syscall.h>
#include <lock.h>

/*
 *swi dispatcher, *arg is the address of sp in swi_handler.S
 */
void c_swi_handler(int swi_num, size_t *args) {
    printf("enter into c_swi_hancler\n");
    switch(swi_num) {
        case READ_SWI:
            // execute each function, store the return value to args[0] 
            // in order to return it to user space
            args[0] = read_syscall((int)args[0], (void *)args[1], (size_t)args[2]);
            break;
        case WRITE_SWI:
            args[0] = write_syscall((int)args[0], (void *)args[1], (size_t)args[2]);
            break;
        case SLEEP_SWI:
            sleep_syscall((unsigned long)args[0]);
            break;
        case TIME_SWI:
            args[0] = time_syscall();
            break;
        case EVENT_WAIT:
            args[0] = event_wait((unsigned int)args[0]);
            break;
        case CREATE_SWI:
            printf("enter into c_swi_hancler: CREATE_SWI\n");
            args[0] = task_create((task_t*)args[0], (size_t)args[1]);
            break;
        case MUTEX_CREATE:
            args[0] = mutex_create();
            break;
        case MUTEX_LOCK:
            args[0] = mutex_lock((int)args[0]);
            break;
        case MUTEX_UNLOCK:
            args[0] = mutex_unlock((int)args[0]);
            break;
        default:
            //invalid swi_number, print error message and exit
            printf("Invalid System Call\n");
            invalid_syscall(swi_num);
    }
}
