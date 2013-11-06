/** @file typo.c
*
* @brief Echos characters back with timing data.
* Authors: DANLEI ZHOU 1 <danleiz> 
*          ZHUOLIN LIU 2 <zhuolinl>
*          QIAN MAO 3 <qianm>
* Links to libc.
*/
#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 1024
int main(int argc, char** argv)
{
    while(1){
        char pompt = '<';
        write(STDOUT_FILENO, (void*) &pompt, 1);
        char str[BUF_SIZE];
        int starttime;
        int currtime;

        starttime = time();
        
        int read_size = read(STDIN_FILENO, str, BUF_SIZE);

        if (read_size < 0) {
        // return error
            return 1;
        }
        currtime = time();
       
        char* write_buf_ptr;
        int write_left, write_bytes;
        write_buf_ptr = str;
        write_left = read_size;

        // make sure output all of input before receiving next input
        while (write_left > 0) {
            write_bytes = write(STDOUT_FILENO, write_buf_ptr, write_left);
                
            // handle remaining stuff
            write_left -= write_bytes;
            write_buf_ptr += write_bytes;
        }

        int diff_int = (currtime- starttime) / 1000;
        int diff_float = (currtime - starttime) / 100 - 10 * diff_int;
        printf("%d.%ds\n", diff_int, diff_float);
        
    }

    return 0;
}
