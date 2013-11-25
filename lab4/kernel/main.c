/*
 * kernel.c: run the kernel to hijacking the U-BOOT's SWI_HANDLER
 * and insert our new handler into it 
 *
 * Authors: ZHUOLIN LIU 1 <zhuolinl>
 *          QIAN MAO 2 <qianm>
 *          DANLEI ZHOU 3 <danleiz> 
 * Date:    22:10 & 11/17/2013
 */

#include <kernel.h>
#include <task.h>
#include <sched.h>
#include <device.h>
#include <assert.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/interrupt.h>
#include <arm/timer.h>
#include <arm/reg.h>
#include <exports.h>
#include <bits/stack.h>
#include <syscall.h>
#include <lock.h>

#define INSTUCT_MASK 0xe59ff000
#define LOAD_PC 0xe51ff004
#define SIGN_MASK 0x00800000
#define PC_AHEAD 0x08
#define OFF_MASK 0xfff

uint32_t global_data;
size_t irq_sp;
size_t saved_sp_add;
size_t saved_kernel_cpsr;
size_t systime = 0;

size_t handler_install(size_t vec_loc, size_t handler_func, size_t *cache_inst_1, size_t *cache_inst_2);
void handler_restore(size_t handler_loc, size_t *cache_inst_1, size_t *cache_inst_2);

int kmain(int argc __attribute__((unused)), char** argv  __attribute__((unused)), uint32_t table)
{

	app_startup();
   

	global_data = table;
	
    size_t swi_handler_loc, irq_handler_loc;
    ssize_t return_status; 
    size_t irq_cache_inst_1, irq_cache_inst_2, swi_cache_inst_1, swi_cache_inst_2;
    size_t vec_loc = SWI_VEC_LOC;
    
    //install two handlers
    swi_handler_loc = handler_install(vec_loc, (size_t)&swi_handler, &swi_cache_inst_1, &swi_cache_inst_2);
    
    // handler install fault, return 0
    if (swi_handler_loc == 0) {
        return 0;
    }

    vec_loc = IRQ_VEC_LOC;
    irq_handler_loc = handler_install(vec_loc, (size_t)&irq_wrapper, &irq_cache_inst_1, &irq_cache_inst_2);

    // handler install fault, return 0
    if (irq_handler_loc == 0) {
        return 0;
    }

    //initialize timer
    reg_clear(INT_ICLR_ADDR, 1 << INT_OSTMR_0);
    reg_set(INT_ICMR_ADDR, 1 << INT_OSTMR_0);
    //enable OSMR in OIER
    reg_set(OSTMR_OIER_ADDR, OSTMR_OIER_E0);
    //clear OSCR
    reg_write(OSTMR_OSCR_ADDR, 0);
    //OSMR0 to be 10ms
    reg_write(OSTMR_OSMR_ADDR(0), T10MS);

    mutex_init();
    
    //push u-boot's argc and argv on the user stack
    return_status = exec(argc, argv);

    //restore the code we try to revise : no need in the project, but leave it
    handler_restore(swi_handler_loc, &swi_cache_inst_1, &swi_cache_inst_2);
    handler_restore(irq_handler_loc, &irq_cache_inst_1, &irq_cache_inst_2);

    return return_status;
}

/*
 * install a handler in some place 
 */
size_t handler_install(size_t vec_loc, size_t handler_func, size_t *cache_inst_1, size_t *cache_inst_2) {
    //get the offset and its sign
    size_t instruction;
    size_t offset;
    size_t sign;
    size_t handler_loc;

    instruction = *(size_t *)vec_loc;
    offset = instruction & OFF_MASK;
    sign = instruction && SIGN_MASK;

    //if instruction is not 'ldr pc, [pc, #imm12]' return error
    if (((instruction - offset) | SIGN_MASK) != INSTUCT_MASK) {
        printf("Unrecognized vector\n");
        return 0;
    }

    //if offset is positive, add the location, else substract it.
    if (sign) {
        handler_loc = *(size_t *)(vec_loc + offset + PC_AHEAD);
    } else {
        handler_loc = *(size_t *)(vec_loc - offset + PC_AHEAD);
    }

    *cache_inst_1 = *(size_t *)handler_loc;
    *cache_inst_2 = *(size_t *)(handler_loc + 4);
       
    *(size_t *)handler_loc = LOAD_PC;
    *(size_t *)(handler_loc + 4) = (size_t)handler_func;

    return handler_loc;
}

/*
 * restore the original instructions which are modified
 */
void handler_restore(size_t handler_loc, size_t *cache_inst_1, size_t *cache_inst_2) {
    *(size_t *)handler_loc = *cache_inst_1;
    *(size_t *)(handler_loc + 4) = *cache_inst_2;
}
