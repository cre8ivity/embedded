/** @file ub_test.c
 *
 * @brief check if ub_test works
 * 
 * @author Qian Mao
 * @date   2013-11-27
 */
#include <stdio.h>
#include <task.h>
#include <unistd.h>
#include <stdio.h>


#define YES_STR "\033[32;1mYes!\033[0m\n"
#define NO_STR  "\033[31;1mNo!\033[0m\n"
#define TEST_DONE "\033[32;1mTEST PASSED SUCCESSFULLY!\033[0m\n"

void panic(const char* str)
{
	puts(str);
	while(1);
}

void fun(void* str)
{
	while(1)
	{	
		putchar((int)str);
		if (event_wait((int)str) < 0)
			panic("Dev 0 failed");
	}
}



int main(int argc, char** argv)
{
	puts("Test begins");
	int i;
	task_t tasks[3];
	for (i = 0; i < 3; i++) {
		tasks[i].lambda = fun;
		tasks[i].data = (void*)i;
		tasks[i].stack_pos = (void*)(0xa2000000 + 0x10000 * i);
	}
	tasks[0].C = 100;
	tasks[0].T = 350;
	tasks[0].B = 0;
	tasks[1].C = 40;
	tasks[1].T = 150;
	tasks[1].B = 0;
	tasks[2].C = 20;
	tasks[2].T = 100;
	tasks[2].B = 0;

	task_create(tasks, 3);
	argc=argc; /* remove compiler warning */
	argv=argv; /* remove compiler warning */

	puts("Elvis could not leave the building, but why did your code get here!\n");
	return 0;
}
