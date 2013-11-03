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
 #include <arm/reg.h>
#include <systemcall.h>

#define INSTUCT_MASK 0xe59ff000
#define LOAD_PC 0xe51ff004
#define SIGN_MASK 0x00800000
#define PC_AHEAD 0x08
#define OFF_MASK 0xfff

uint32_t global_data;
size_t saved_sp_add;

size_t handler_install(size_t vec_loc, size_t handler_func, size_t[] cache);
void handler_restore(size_t handler_loc, size_t[] cache);

int kmain(int argc, char** argv, uint32_t table)
{
	app_startup(); /* bss is valid after this point */
	global_data = table;

	/* Add your code here */
    size_t swi_handler_loc, irq_handler_loc;
    ssize_t return_status; 
    size_t[2] irq_cache, swi_cache;
    size_t vec_loc = SWI_VEC_LOC;
    
    //install two handlers
    swi_handler_loc = handler_install(vec_loc, &swi_handler, swi_cache);
    vec_loc = IRQ_VEC_LOC;
    irq_handler_loc = handler_install(vec_loc, &irq_handler, irq_cache);

    //initialize timer
    reg_clear(INT_ICLR_ADDR, 1 << (INT_OSTMR_0-1));
    reg_set(INT_ICMR_ADDR, 1 << (INT_OSTMR_0-1));
    //enable OSMR in OIER
    reg_set(OSTMR_OIER_ADDR, OSTMR_OIER_E0);
    //clear OSCR
    reg_write(OSTMR_OSCR_ADDR, 0);
    //OSMR0 to be 10ms
    reg_write(OSTMR_OSMR_ADDR(0), T10MS);

    
    //push u-boot's argc and argv on the user stack
    return_status = exec(argc, argv);

    //restore the code we try to revise
    handler_restore(swi_handler_loc, swi_cache);
    handler_restore(irq_handler_loc, irq_cache);

    return return_status;
}


size_t handler_install(size_t vec_loc, size_t handler_func, size_t[] cache) {
    //get the offset and its sign
    size_t instruction = *(size_t *)vec_loc;
    size_t offset = instruction & OFF_MASK;
    size_t sign = instruction && SIGN_MASK;
    size_t handler_loc, cache_inst_1, cache_inst_2;

    //if instruction is not 'ldr pc, [pc, #imm12]' return error
    if (((instruction - offset) | SIGN_MASK) != INSTUCT_MASK) {
        printf("Unrecognized vector\n");
        exit(DEFAULT_EXIT);
    }

    //if offset is positive, add the location, else substract it.
    if (sign) {
        handler_loc = *(size_t *)(vec_loc + offset + PC_AHEAD);
    } else {
        handler_loc = *(size_t *)(vec_loc - offset + PC_AHEAD);
    }

    cache[0] = *(size_t *)handler_loc;
    cache[1] = *(size_t *)(handler_loc + 4);
       
    *(size_t *)handler_loc = LOAD_PC;
    *(size_t *)(handler_loc + 4) = (size_t)handler_func;

    return handler_loc;
}

void handler_restore(size_t handler_loc, size_t[] cache) {
    *(size_t *)handler_loc = cache[0];
    *(size_t *)(handler_loc + 4) = cache[1];
}
