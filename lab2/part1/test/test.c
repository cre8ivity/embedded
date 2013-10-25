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
    int read_bytes;
    //int write_bytes;
    char buf[BUF_SIZE];     // block buffer
    //char *write_buf_ptr;    // shifting buf ptr for write
    //int write_left;         // remaining chars to be written 
   
    // read bytes
    read_bytes = read(1, buf, BUF_SIZE);
    char dig = (char)(((int)'0') + read_bytes);
    

    if ((write_bytes = write(STDOUT_FILENO, &dig, )) < 0) {
                exit(1);
        }
    if (read_bytes == 1) {
            exit(0);
    }

    if (read_bytes < 0) {
        // return error       
        return read_bytes;
    }

    return 0;
}
