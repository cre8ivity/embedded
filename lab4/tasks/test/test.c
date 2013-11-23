/** @file test.c
 *
 * @brief Creates two simple mutex tasks.
 *
 * @note  This is like knife -- but smaller :)
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date   2008-11-30
 */
#include <stdio.h>
#include <task.h>
#include <unistd.h>
#include <lock.h>

// only fun1 can create the mutex
int mutex_use = -1;

void panic(const char* str)
{
    puts(str);
    while(1);
}

void fun1(void* str)
{
    mutex_use = mutex_create();
    while(1)
    {
        putchar((int)str);
        mutex_lock(mutex_use);
        puts("I am in fun1, going to sleep!");
        sleep(1000);
        puts("I am awake from fun1");
        mutex_unlock(mutex_use);
    }
}

void fun2(void* str)
{
    // let fun1 create the mutex
    sleep(3000);
    while(1)
    {
        putchar((int)str);
        mutex_lock(mutex_use);
        puts("I am in fun2, going to sleep!");
        sleep(1000);
        puts("I am awake from fun2");
        mutex_unlock(mutex_use);
    }
}

int main(int argc, char** argv)
{
    task_t tasks[2];
    tasks[0].lambda = fun1;
    tasks[0].data = (void*)'@';
    tasks[0].stack_pos = (void*)0xa2000000;
    tasks[0].C = 1;
    tasks[0].T = PERIOD_DEV0;
    tasks[1].lambda = fun2;
    tasks[1].data = (void*)'<';
    tasks[1].stack_pos = (void*)0xa1000000;
    tasks[1].C = 1;
    tasks[1].T = PERIOD_DEV1;

    task_create(tasks, 2);
    argc=argc; /* remove compiler warning */
    argv[0]=argv[0]; /* remove compiler warning */

    puts("Why did your code get here!\n");
    return 0;
}
