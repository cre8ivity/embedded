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

#define INSTUCT_MASK 0xe59ff000
#define LOAD_PC 0xe51ff004

int main(int argc, char *argv[]) {
	//get the location of swi handler from vector table
    size_t instruction = *(size_t *)&(SWI_VEC_LOC);
    //get the offset and its sign
    size_t offset = instruction & 0xfff;
    size_t sign = instuction && 0x00800000;
    char error_msg[30] = "Unrecognized SWI vector\n";
    size_t swi_handler_loc, cache_intruc_1, cache_intruc_2;
    ssize_t user_return_value;

    //if instruction is not 'ldr pc, [pc, #imm12]' return error
    if ((instuction - offset) | sign != INSTUCT_MASK) {
        write(STDOUT_FILENO, error_mag, 24);
        exit(DEFAULT_EXIT);
    }

    //if offset is positive, add the location, else substract it.
    if (sign) {
        swi_handler_loc = *(size_t *)&(SWI_VEC_LOC + offset + 0x08);
    } else {
        swi_handler_loc = *(size_t *)&(SWI_VEC_LOC - offset + 0x08);
    }

    //store the code we try to revise
    cache_intruc_1 = *(size_t *)swi_handler_loc;
    cache_intruc_2 = *(size_t *)(swi_handler_loc + 4);

    //change the code
    //ldr pc, [pc, #-4]
    *(size_t *)swi_handler_loc = LOAD_PC;
    *(size_t *)(swi_handler_loc + 4) = (size_t)&swi_handler;

    //push u-boot's argc and argv on the user stack
    user_return_value = pushing_arg(argc, argv);

    //restore r8
    restore_r8();

    //restore the code we try to revise
    *(size_t *)swi_handler_loc = cache_intruc_1;
    *(size_t *)(swi_handler_loc + 4) = cache_intruc_2;

	return user_return_value;
}
