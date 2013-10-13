/*
 * c_swi_handler.c: swi dispatcher
 *
 * Authors: ZHUOLIN LIU 1 <zhuolinl>
 *          QIAN MAO 2 <qianm>
 *          DANLEI ZHOU 3 <danleiz> 
 * Date:    17:45 & 10/11/2013
 */
#include <bits/types.h>
#include <bits/fileno.h>
#include <bits/stack.h>
#include <exports.h>
#include <systemcall.h>

/*
 *swi dispatcher, *arg is the address of sp in swi_handler.S
 */
void c_swi_handler(int swi_num, void *args) {
    switch(swi_num) {
        case READ_SWI:
            //store the return value to args[0] in order to return it 
            //to user space
            ((int)args[0]) = read((int)args[0], &args[1], (size_t)args[2]);
            break;
        case WRITE_SWI:
            ((int)args[0]) = write((int)args[0], &args[1], (size_t)args[2]);
            break;
        case EXIT_SWI:
            exit((int)args[0]);
            break;
        default:
            //invalid swi_number, print error message and exit
            char buf[25] = "Invalid System Call\n";
            write(STDOUT_FILENO, buf, 20);
            exit(DEFAULT_EXIT);
    }
}
