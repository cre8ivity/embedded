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
#include <exports.h>
#include <systemcall.h>

#define DEFAULT_EXIT 0x0badc0de

//swi dispatcher, *arg is the address of sp in swi_handler.S
void c_swi_handler(int swi_num, void *args) {
    switch(swi_num) {
        case READ_SWI:
            read((int)args[0], &args[1], args[2]);
            break;
        case WRITE_SWI:
            write((int)args[0], &args[1], args[2]);
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