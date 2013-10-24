/*
 * kernel.c: run the kernel to hijacking the U-BOOT's SWI_HANDLER
 * and insert our new handler into it 
 *
 * Authors: ZHUOLIN LIU 1 <zhuolinl>
 *          QIAN MAO 2 <qianm>
 *          DANLEI ZHOU 3 <danleiz> 
 * Date:    23:45 & 10/11/2013
 */

#include <bits/stack.h>
#include <bits/types.h>
#include <bits/fileno.h>
#include <systemcall.h>
#include <exports.h>

#define INSTUCT_MASK 0xe59ff000
#define LOAD_PC 0xe51ff004


int main(int argc, char *argv[]) {
    printf("get the location of swi handler from vector table\n");
    size_t instruction = *(size_t *)SWI_VEC_LOC;
    //get the offset and its sign
    size_t offset = instruction & 0xfff;
    size_t sign = instruction && 0x00800000;
    size_t swi_handler_loc, cache_inst_1, cache_inst_2;
    ssize_t return_status;

    //if instruction is not 'ldr pc, [pc, #imm12]' return error
    //printf("instruction: %s, sign: %s\n", instruction, sign);
    /*
    if (((instruction - offset) | sign) != INSTUCT_MASK) {
        printf("Unrecognized SWI vector\n");
        exit(DEFAULT_EXIT);
    }
    */

    //if offset is positive, add the location, else substract it.
    if (sign) {
        swi_handler_loc = *(size_t *)(SWI_VEC_LOC + offset + 0x08);
    } else {
        swi_handler_loc = *(size_t *)(SWI_VEC_LOC - offset + 0x08);
    }

    printf("store the code we try to revise\n");
    cache_inst_1 = *(size_t *)swi_handler_loc;
    cache_inst_2 = *(size_t *)(swi_handler_loc + 4);
    
    printf("cache_inst_1: %x\n", cache_inst_1);
    printf("cache_inst_2: %x\n", cache_inst_2);
    //change the code
    //ldr pc, [pc, #-4]
    *(size_t *)swi_handler_loc = LOAD_PC;
    *(size_t *)(swi_handler_loc + 4) = (size_t)&swi_handler;

    //
    printf("enter into user program\n");

    //push u-boot's argc and argv on the user stack
    return_status = pushing_arg(argc, argv);

    //
    printf("exit from user program\n");

    //restore r8
    //restore_r8();

    //restore the code we try to revise
    *(size_t *)swi_handler_loc = cache_inst_1;
    printf("restored first\n");
    *(size_t *)(swi_handler_loc + 4) = cache_inst_2;
    printf("resorted second\nstatus:%d\n", return_status);
    return return_status;
}
