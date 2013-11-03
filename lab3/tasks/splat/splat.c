/** @file splat.c
 *
 * @brief Displays a spinning cursor.
 *
 * Links to libc.
 */
#include<stdio.h>
int main(int argc, char** argv)
{
	/* Add code here -- put your names at the top. */
	
	char c[] = "|/-\\";
	int i = 0;
	while(1){

		printf("%c",c[i]);		
		
		sleep(200);
		printf("\b");
		
		i = i==3 ?0:i+1; 
			
	}

        return 0;
}
