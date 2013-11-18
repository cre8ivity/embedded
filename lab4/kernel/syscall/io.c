/** @file io.c
 *
 * @brief Kernel I/O syscall implementations
 *
 * @author Mike Kasick <mkasick@andrew.cmu.edu>
 * @date   Sun, 14 Oct 2007 00:07:38 -0400
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-16
 */

#include <types.h>
#include <bits/errno.h>
#include <bits/fileno.h>
#include <bits/stack.h>
#include <arm/physmem.h>
#include <syscall.h>
#include <exports.h>
#include <kernel.h>

#define EOT 4
//#define DELETE 127
#define BACKSPACE 127
#define NEWLINE 10
#define CARRY_RETURN 13


/* Read count bytes (or less) from fd into the buffer buf. */
ssize_t read_syscall(int fd, void *buf, size_t count)
{
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

/* Write count bytes to fd from the buffer buf. */
ssize_t write_syscall(int fd, const void *buf, size_t count)
{
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

