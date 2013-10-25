/*
 * stack.h: Defines the stack location in U-BOOT Environment
 *
 * Authors: ZHUOLIN LIU 1 <zhuolinl>
 *          QIAN MAO 2 <qianm>
 *          DANLEI ZHOU 3 <danleiz> 
 * Date:    14:45 & 10/11/2013
 */

#ifndef BITS_STACK_H
#define BITS_STACK_H

#define SDRAM_BASE  0xa0000000
#define SDRAM_END   0xa3ffffff
#define ROM_BASE    0x00000000
#define ROM_END     0x00ffffff
#define SWI_VEC_LOC 0x08
#define USER_MODE_STACK 0xa3000000
#define USER_PRG_ADD 0xa2000000
#define DEFAULT_EXIT 0x0badc0de
#define HEAP_BASE   0xa3edf000
#define HEAP_END    0xa3efffff
#define FREE_BASE   0xa0000000
#define FREE_END    0xa1ffffff 

#endif /* BITS_STACK_H */
