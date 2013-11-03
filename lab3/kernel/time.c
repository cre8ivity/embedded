/*
 * time.c: time system call
 *
 * Authors: ZHUOLIN LIU 1 <zhuolinl>
 *          QIAN MAO 2 <qianm>
 *          DANLEI ZHOU 3 <danleiz> 
 * Date:    1:28 & 11/2/2013
 */
#include <types.h>
#include <arm/timer.h>
#include <lib.h>

extern volatile uint64_t systime;

unsigned long time() {

    return systime * TRUE_TIME_RATIO;
}

