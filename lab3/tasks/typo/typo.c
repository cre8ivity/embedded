/** @file typo.c
*
* @brief Echos characters back with timing data.
*
* Links to libc.
*/
#include <stdio.h>
//#include <time.h>
#include <unistd.h>
#define BUF_SIZE 1024
int main(int argc, char** argv)
{
    
    while(1){
<<<<<<< HEAD
        char pompt = '<';
=======
        char pompt = '<';   
>>>>>>> e3f88b8b861d54d8a4775d801c9b054e0d8acf1e
        write(1,(void* )&pompt,1);
        char str[BUF_SIZE];
        int starttime;
        int currtime;

        starttime = time();
        // TODO: need to handle case when user input long strings
        int read_size = read(0, str, BUF_SIZE);
        currtime = time();
        int diff = currtime- starttime;
        write(1,(void* )&str,read_size);
        printf("%d\n",diff);
    }
    return 0;
}
