/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 * Authors: DANLEI ZHOU 1 <danleiz> 
 *          ZHUOLIN LIU 2 <zhuolinl>
 *          QIAN MAO 3 <qianm>
 * Links to libc.
 */
#include <stdio.h>
#include <unistd.h>

#define SLEEPTIME 200

int main(int argc, char** argv)
{
    // initiall the char to splat
    char c[] = "|/-\\";
    int i = 0;

    while(1){
        //print the char and sleep for a while
        printf("%c",c[i]);
        sleep(SLEEPTIME);

        //backspace to print a new char
        printf("\b");
        
        i = i == 3 ? 0 : i+1; 	
    }

    return 0;
}
