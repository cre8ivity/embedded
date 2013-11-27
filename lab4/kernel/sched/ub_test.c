/** @file ub_test.c
 * 
 * @brief The UB Test for basic schedulability
 *
 * @author Kartik Subramanian <ksubrama@andrew.cmu.edu>
 * @date 2008-11-20
 */

//#define DEBUG 0

#include <sched.h>
#ifdef DEBUG
#include <exports.h>
#endif

 
/**
 * @brief Perform UB Test and reorder the task list.
 *
 * The task list at the end of this method will be sorted in order is priority
 * -- from highest priority (lowest priority number) to lowest priority
 * (highest priority number).
 *
 * @param tasks  An array of task pointers containing the task set to schedule.
 * @param num_tasks  The number of tasks in the array.
 *
 * @return 0  The test failed.
 * @return 1  Test succeeded.  The tasks are now in order.
 */

static double[64] ub_values = {
    1.000000,  0.828427,  0.779763,  0.756828,  0.743492,  0.734772,  0.728627,  0.724062,  
    0.720538,  0.717735,  0.715452,  0.713557,  0.711959,  0.710593,  0.709412,  0.708381,  
    0.707472,  0.706666,  0.705946,  0.705298,  0.704713,  0.704182,  0.703698,  0.703254,  
    0.702846,  0.702469,  0.702121,  0.701798,  0.701497,  0.701217,  0.700955,  0.700709,  
    0.700478,  0.700261,  0.700056,  0.699863,  0.699681,  0.699508,  0.699343,  0.699188,  
    0.699040,  0.698898,  0.698764,  0.698636,  0.698513,  0.698396,  0.698284,  0.698176,  
    0.698073,  0.697974,  0.697879,  0.697788,  0.697700,  0.697615,  0.697533,  0.697455,  
    0.697379,  0.697306,  0.697235,  0.697166,  0.697100,  0.697036,  0.696974,  0.696914 }


/*
 * First off, sort the input list so that it satisfies rate-monotonicity. 
 * Then run the UB test on it to see if the given task set is schedulable.
 */
int assign_schedule(task_t** tasks  __attribute__((unused)), size_t num_tasks  __attribute__((unused)))
{
    int i, result = 1;
    double sum = 0.0, u_value;
    unsigned long task_array[num_tasks];

    for (i = 0; i < num_tasks; i++) {
        task_array[i] = task[i]->T;
    }
    // sort based on T
    qsort(task_array, tasks, 0, num_tasks - 1);

    // UB test
    for (i = 0; i < num_tasks; i++) {
    	u_value = sum + (tasks[i]->C + tasks[i]->B) * 1.0 / tasks[i]->T;
    	if (u_value > ub_values[i]) {
    		result = 0;
    		break;
    	}
    	sum += tasks[i]->C * 1.0 / tasks[i]->T;
    }

	return result;
}



static void qsort(int task_prio[], task_t** task_ptr, int l, int r)
{
    int i, j, x;
    task_t *xp;
    if (l < r) {
        i = l;
        j = r;
        x = task_prio[i];
        xp = task_ptr[i];
        while (i < j)
        {
            while(i < j && task_prio[j] > x)  {
                j--;
            }

            if(i < j) { 
                task_prio[i] = task_prio[j];
                task_ptr[i++] = task_ptr[j];
            }

            while(i < j && task_prio[i] < x) {
                i++; 
            }

            if(i < j) {  
                task_prio[j = task_prio[i];
                task_ptr[j--] = task_ptr[i];
            }
        }

        task_prio[i] = x;
        task_ptr[i] = xp;
        qsort(task_prio, task_ptr, l, i-1); 
        qsort(task_prio, task_ptr, i+1, r);
    }
}

