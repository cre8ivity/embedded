/** @file typo.c
 *
 * @brief Echos characters back with timing data.
 *
 * Links to libc.
 */
#include <stdio.h>
#include <time.h>
int main(int argc, char** argv)
{
	
	while(1){
		char pompt = '<';	
		putchar(pompt);
		char str[1000];
	
		int starttime;
		int currtime;

		starttime = time(NULL);
		scanf("%s",str);
		currtime = time(NULL);
		int diff = currtime- starttime;
	    puts(str);
	    printf("%d\n",diff);
    }
	return 0;
}
