/*
 * stack.h: Defines the stack location in U-BOOT Environment
 *
 * Authors: ZHUOLIN LIU 1 <zhuolinl>
 *          QIAN MAO 2 <qianm>
 *          DANLEI ZHOU 3 <danleiz> 
 * Date:    14:45 & 10/11/2013
 */

#ifdef SYSTERMCALL_H
#define SYSTERMCALL_H

ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, void *buf, size_t count);
void exit(int status);
void swi_handler();

#endif /* SYSTERMCALL_H */