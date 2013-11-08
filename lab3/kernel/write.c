/*
 * write.c: write system call
 *
 * Authors: ZHUOLIN LIU 1 <zhuolinl>
 *          QIAN MAO 2 <qianm>
 *          DANLEI ZHOU 3 <danleiz> 
 * Date:    14:45 & 10/11/2013
 */
#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <bits/stack.h>
#include <exports.h>
#include <lib.h>

ssize_t write(int fd, void *buf, size_t count) {
    size_t i;
    ssize_t return_count;
    char *temp_buf = (char *)buf;
    char newline = '\n';

    //if fd is not STDOUT, return -EBADF
    if (fd != STDOUT_FILENO) {
        return -EBADF;
    }

    //buf should always be in the sdram and rom space
    //use minus to prevent overflow
    if ( ((size_t)buf >= SDRAM_BASE && (size_t)buf <= SDRAM_END &&
        SDRAM_END - (size_t)buf + 1 >= count) ||
        // ((size_t)buf >= ROM_BASE && (size_t)buf <= ROM_END && 
        ((size_t)buf <= ROM_END && 
        // ROM_BASE is 0, so no need to compare it with usigned int        
        ROM_END - (size_t)buf + 1 >= count)) {

        //write 
        return_count = 0;
        for (i = 0; i < count; i++) {
            putc(temp_buf[return_count]);
            if(temp_buf[return_count] == '\r')
		        putc(newline);
            return_count++;
        }
    } else {
        //if memory range outside the SDRAM or ROM, return -EFAULT
        return -EFAULT;
    }

    //return number of bytes write
    return count;
}
