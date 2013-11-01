/*
 * read.c: read system call
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

#define EOT 4
//#define DELETE 127
#define BACKSPACE 127
#define NEWLINE 10
#define CARRY_RETURN 13

ssize_t read(int fd, void *buf, size_t count) {
    size_t i;
    ssize_t return_count;
    char temp_char;
    char *temp_buf = (char *)buf;

    //if fd is not STDIN, return -EBADF
    if (fd != STDIN_FILENO) {
        return -EBADF;
    }

    //if memory range outside the HEAP or FREE range, return -EFAULT
    //use minus to prevent overflow
    if (!(((size_t)buf >= HEAP_BASE && (size_t)buf <= HEAP_END &&
        HEAP_END - (size_t)buf + 1 >= count) ||
        ((size_t)buf >= FREE_BASE && (size_t)buf <= FREE_END &&
        FREE_END - (size_t)buf + 1 >= count))) {
        return -EFAULT;
    }

    //retrun characters into the buffer from stdin
    return_count = 0;
    for (i = 0; i < count; i++) {
        temp_char = getc();
        
        switch(temp_char) {
            case EOT: //EOT return i
                return return_count++;
            //case DELETE:
            case BACKSPACE:
                return_count--; // remove the current char
                //remove the previous char when i is not 0
                if (return_count != -1) {
                    temp_buf[return_count] = 0;
                    puts("\b \b");
                    return_count--;
                }
                break;
            case NEWLINE:
            case CARRY_RETURN:
            //return number of characters when \n\r
                temp_buf[return_count] = temp_char;
                putc('\n');
                return return_count+1;
            default:
            //return 1 character
                temp_buf[return_count] = temp_char;
                putc(temp_char);
        }
        return_count++;
    }

    //return number of bytes read
    return count;
}
