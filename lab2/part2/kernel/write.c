/*
 * write.c: write system call
 *
 * Authors: ZHUOLIN LIU 1 <zhuolinl>
 *          QIAN MAO 2 <qianm>
 *          DANLEI ZHOU 3 <danleiz> 
 * Date:    14:45 & 10/11/2013
 */
#include <bits/types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <bits/stack.h>
#include <exports.h>

ssize_t write(int fd, void *buf, size_t count) {
    int i;
    char *temp_buf = (char *)buf;
    char newline = '\n';

    //if fd is not STDOUT, return -EBADF
    if (fd != STDOUT_FILENO) {
        return -EBADF;
    }

    //buf should always be in the sdram range of rom range
    //use minus to prevent overflow
    if ( ((size_t)buf >= SDRAM_BASE && (size_t)buf <= SDRAM_END &&
        SDRAM_END - (size_t)buf >= count) ||
        ((size_t)buf >= ROM_BASE && (size_t)buf <= ROM_END && 
        ROM_END - (size_t)buf >= count)) {
        //write 
        for (i = 0; i < count; i++) {
            putc(temp_buf[i]);
            if(temp_buf[i] == '\r')
		putc(newline);
        }
    } else {
        //if memory range outside the SDRAM or ROM, return -EFAULT
        return -EFAULT;
    }

    //return number of bytes write
    return count;
}
