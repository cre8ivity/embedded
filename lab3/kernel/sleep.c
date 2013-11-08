/*
 * sleep.c: sleep system call
 *
 * Authors: ZHUOLIN LIU 1 <zhuolinl>
 *          QIAN MAO 2 <qianm>
 *          DANLEI ZHOU 3 <danleiz> 
 * Date:    1:19 & 11/2/2013
 */
#include <types.h>
#include <arm/timer.h>
#include <lib.h>

extern volatile uint64_t systime;

void sleep(unsigned long millis) {

    uint64_t curr_time = systime;
    // while until the current time is over the setted time
    while (curr_time + millis / TRUE_TIME_RATIO >= systime);
}

