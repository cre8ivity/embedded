/*
 * rot13.c: ROT13 cipher test application
 *
 * Authors: ZHUOLIN LIU 1 <zhuolinl>
 *          QIAN MAO 2 <qianm>
 *          DANLEI ZHOU 3 <danleiz> 
 * Date:    21:45 & 10/9/2013
 */

#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(void) {
    int read_bytes, write_bytes;
    char buf[BUF_SIZE];     // block buffer
    char *write_buf_ptr;     // shifting buf ptr for write
    int write_left;         // remaining chars to be written 
    int i;

    while(1) {
        // read bytes
        read_bytes = read(STDIN_FILENO, buf, BUF_SIZE);
        // exit if no input
        if (read_bytes == 0) {
            exit(0);
        }

        do {
            if (read_bytes < 0) {
            // return error
                exit(1);
            }
            // go through and rot 13
            for (i = read_bytes - 1; i >= 0; i--) {
                // rot 13
                if ((buf[i] >= 65 && buf[i] <= 77) ||
                    (buf[i] >= 97 && buf[i] <= 109)) {
                    buf[i] += 13;
                } else if ((buf[i] >= 78 && buf[i] <= 90) ||
                    (buf[i] >= 110 && buf[i] <= 122)) {
                    buf[i] -= 13;
                } 
            }

            // robuust output
            write_buf_ptr = buf;
            write_left = read_bytes;

            // make sure output all stuff before receiving next input
            while (write_left > 0) {
                if ((write_bytes = write(STDOUT_FILENO, write_buf_ptr, write_left)) < 0) {
                    exit(1);
                }
                // handle remainning stuff
                write_left -= write_bytes;
                write_buf_ptr += write_bytes;
            }

        //read bytes if it more than buffer length
        } while ((read_bytes = read(STDIN_FILENO, buf, BUF_SIZE)) != 0);
    }

    return 0;
}
