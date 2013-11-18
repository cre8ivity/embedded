/*
 * c_irq_handler: irq handle function
 *
 * Authors: ZHUOLIN LIU 1 <zhuolinl>
 *          QIAN MAO 2 <qianm>
 *          DANLEI ZHOU 3 <danleiz> 
 * Date:    17:45 & 10/11/2013
 */
#include <types.h> 
#include <arm/reg.h>
#include <arm/timer.h>
#include <arm/interrupt.h>

extern volatile uint64_t systime;

void irq_handler() {
    // figure out if the irq is filed by OMSR0 match
    uint32_t is_timerInterrupt = reg_read(INT_ICPR_ADDR) & (1 << (INT_OSTMR_0));
    uint32_t updated_match;
    
    if (is_timerInterrupt) {
        // 10ms heart bit, try to update the match regsiter
        updated_match = reg_read(OSTMR_OSCR_ADDR) + T10MS;
        reg_write(OSTMR_OSMR_ADDR(0), updated_match);
        // clear OSSR match and update system time
        reg_set(OSTMR_OSSR_ADDR, OSTMR_OSSR_M0); 
        systime++;

        // update device whether nect event for every device has occured 
        dev_update(get_millis());

        // context switch
        dispatch_save();
    }
}
