/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: ZHUOLIN LIU 1 <zhuolinl>
 *          QIAN MAO 2 <qianm>
 *          DANLEI ZHOU 3 <danleiz> 
 * Date:    14:45 & 10/11/2013
 */
#include <bits/types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <exports.h>

ssize_t write(int fd, void *buf, size_t count) {
    int i;

    //if fd is not STDOUT, return -EBADF
    if (fd != STDOUT_FILENO) {
        return -EBADF;
    }

    if ( (buf >= SDRAM_BASE && buf <= SDRAM_END &&
        SDRAM_END - (size_t)buf >= count) ||
        (buf >= ROM_BASE && buf <= ROM_END && 
        ROM_END - (size_t)buf >= count)) {
        //write 
        for (i = 0; i < count; i++) {
            putc(buf[i]);
        }
    } else {
        //if memory range outside the SDRAM or ROM, return -EFAULT
        return -EFAULT
    }

    //return number of bytes write
    return count;
}