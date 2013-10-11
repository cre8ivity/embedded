/*
 * read.c: read system call
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

#define EOT 4
#define DELETE 127
#define BACKSPACE 8
#define NEWLINE 10
#define CARRY_RETURN 13

ssize_t read(int fd, void *buf, size_t count) {
    int i;
    char temp_char;

    //if fd is not STDIN, return -EBADF
    if (fd != STDIN_FILENO) {
        return -EBADF;
    }

    //if memory range outside the SDRAM, return -EFAULT
    if (buf < SDRAM_BASE || buf > SDRAM_END
        || SDRAM_END - (size_t)buf < count) {
        return -EFAULT;
    }

    //retrun characters into the buffer from stdin
    for (i = 0; i < count; i++) {
        temp_char = getc();

        switch(temp_char) {
            case EOT: //EOT return i
                return i;
            case DELETE:
            case BACKSPACE:
                i--; // remove the current char
                //remove the previous char when i is not 0
                if (i != -1) {
                    buf[i] = 0;
                    puts("\b \b");
                    i--;
                }
                break;
            case NEWLINE:
            case CARRY_RETURN:
            //return number of characters when \n\r
                buf[i] = temp_char;
                putc(temp_char);
                return i+1;
            default:
            //return 1 character
                buf[i] = temp_char;
                putc(temp_char);
        }
    }

    //return number of bytes read
    return count;
}
