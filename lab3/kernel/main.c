/*
 * kernel.c: run the kernel to hijacking the U-BOOT's SWI_HANDLER
 * and insert our new handler into it 
 *
 * Authors: ZHUOLIN LIU 1 <zhuolinl>
 *          QIAN MAO 2 <qianm>
 *          DANLEI ZHOU 3 <danleiz> 
 * Date:    15:10 & 10/31/2013
 */

#include <bits/stack.h>
#include <exports.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <systemcall.h>

#define INSTUCT_MASK 0xe59ff000
#define LOAD_PC 0xe51ff004

uint32_t global_data;

size_t saved_sp_add;

int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* bss is valid after this point */
	global_data = table;

	/* Add your code here */
    
    size_t instruction = *(size_t *)SWI_VEC_LOC;

    //get the offset and its sign
    size_t offset = instruction & 0xfff;
    size_t sign = instruction && 0x00800000;
    size_t swi_handler_loc, cache_inst_1, cache_inst_2;
    ssize_t return_status;

    //if instruction is not 'ldr pc, [pc, #imm12]' return error
    if (((instruction - offset) | 0x00800000) != INSTUCT_MASK) {
        printf("Unrecognized SWI vector\n");
        exit(DEFAULT_EXIT);
    }

    //if offset is positive, add the location, else substract it.
    if (sign) {
        swi_handler_loc = *(size_t *)(SWI_VEC_LOC + offset + 0x08);
    } else {
        swi_handler_loc = *(size_t *)(SWI_VEC_LOC - offset + 0x08);
    }

    cache_inst_1 = *(size_t *)swi_handler_loc;
    cache_inst_2 = *(size_t *)(swi_handler_loc + 4);
    
    
    *(size_t *)swi_handler_loc = LOAD_PC;
    *(size_t *)(swi_handler_loc + 4) = (size_t)&swi_handler;

    //push u-boot's argc and argv on the user stack
    return_status = exec(argc, argv);

    //restore the code we try to revise
    *(size_t *)swi_handler_loc = cache_inst_1;
    *(size_t *)(swi_handler_loc + 4) = cache_inst_2;
    return return_status;
}
