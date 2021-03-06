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
#include <systemcall.h>
#include <lib.h>

/*
 *swi dispatcher, *arg is the address of sp in swi_handler.S
 */
void c_swi_handler(int swi_num, size_t *args) {
    switch(swi_num) {
        case READ_SWI:
            // execute each function, store the return value to args[0] 
            // in order to return it to user space
            args[0] = read((int)args[0], (void *)args[1], (size_t)args[2]);
            break;
        case WRITE_SWI:
            args[0] = write((int)args[0], (void *)args[1], (size_t)args[2]);
            break;
        case EXIT_SWI:
            exit((int)args[0]);
            break;
        case SLEEP_SWI:
            sleep((unsigned long)args[0]);
            break;
        case TIME_SWI:
            args[0] = time();
            break;
        default:
            //invalid swi_number, print error message and exit
            printf("Invalid System Call\n");
            exit(DEFAULT_EXIT);
    }
}
