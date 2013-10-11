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
    char buf[BUF_SIZE]; //block buffer
    int i;

    while(1) {
        //read bytes
        read_bytes = read(STDIN_FILENO, buf, BUF_SIZE);
        //exit if no input
        if (read_bytes == 0) {
            exit(0);
        }

        do {
            if (read_bytes < 0) {
            //return error
                exit(1);
            }
            //go through and rot 13
            for (i = 0; i < read_bytes; i++) {
                //rot 13
                if ((buf[i] >= 65 && buf[i] <= 77) ||
                    (buf[i] >= 97 && buf[i] <= 109)) {
                    buf[i] += 13;
                } else if ((buf[i] >= 78 && buf[i] <= 90) ||
                    (buf[i] >= 110 && buf[i] <= 122)) {
                    buf[i] -= 13;
                } 
            }

            //output
            write_bytes = write(STDOUT_FILENO, buf, read_bytes);
            //exit if error
            if (write_bytes < 0) {
                exit(1);
            }
        //read bytes if it more than buffer length
        } while ((read_bytes = read(STDIN_FILENO, buf, BUF_SIZE)) != 0);
    }

    return 0;
}
