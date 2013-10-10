/*
 Danlei Zhou(danleiz)
 Zhuolin Liu(zhuolinl)
 Qian Mao(qianm)
 */

#include <stdlib.h>
#include <stdio.h>


/**
 * Function: oddball
 * Description: Baseline implementation for finding the number that
 *   occurs only once in the array.
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   The number that occurs only once
 */
#ifdef DEFAULT
/* 
 * since every element has its place to go, we can
 * put the element to where it should be in each
 * round. If the place has already had the correct
 * value. We can set 0, to indicate success, o/w
 * we can continuously swap the value, until every
 * one is in the right place. Becuase each time, we
 * put a value to the correct place. We should only
 * do 'len'th swap at most, to make everyone be in
 * its place. This method takes O(n) time with no 
 * extra place. 
 * Drawback: the array which has been broken cannot
 * be restored
 */
int oddball(int *arr, int len) {
	//set the iterator
	int i = 0;

	while (i < len) {
		//optimize code by set temp variable
		int real_ai = arr[i] - 1;
		
		//continually swaping until this place has the right element
		//or success(0) or place swaping with has success
		while (arr[i] != 0 && i != real_ai && arr[real_ai] != 0) {
			//the place swaping with has already had right element
			//indicate suceess
			if (arr[real_ai] == arr[i]) {
				arr[real_ai] = 0;
			} else {
			//incorrect element, put it to the right place and swap
				int swap_tmp = arr[real_ai];
				arr[real_ai] = arr[i];
				arr[i] = swap_tmp;
				real_ai = arr[i] - 1;
			}
		}
		i++;
	}
	
	//output the one which does not success
	//(len+1)/2-1 = len >> 1
	for (i = len >> 1; i >=0; i--) {
		if (arr[i] != 0) {
			return i + 1;
		}
	}
	return 0;
}

#endif

#ifdef OPTIMIZE1
/*
 * each element should have its right place. We
 * can go through the array and flip the sign of 
 * the place where arr[i] should be. For example.
 * when we meet abs(arr[i]) = 3, we set arr[2] = 
 * -arr[2], since for every number we will flip
 * its right place for twice and finally get a 
 * positive number except the specially one which
 * only filp once. So we can check the array to 
 * find the position where has a negative number.
 * The time is O(n) for two interation, and we
 * do not need extra space. Also, we can restore
 * the original array if we want.
 */
int oddball(int *arr, int len) {
	int i;
	for (i = len - 1; i >=0; i--) {
	//flip the element where they should be
		int tmp = (arr[i] > 0 ? arr[i] : -arr[i]) - 1;
		arr[tmp] = -arr[tmp];
	}

	//(len+1)/2-1 = len >> 1
	for (i = len >> 1; i >= 0; i--) {
	//return the position has negatibe number
		if (arr[i] < 0) {
			return i + 1;
		}
	}
	return 0;
}
#endif

#ifdef OPTIMIZE2

/*
 * Use XOR to sum all numbers. Due to the feature 
 * of XOR, If a number appears twice, the number 
 * will flip back itself to all zeros. Therefore,
 * after use XOR to sum up all numbers, the result
 * will be the number which appears only once.
 * time complexity: O(n)
 * space complexity: O(1): local varaible
 */
int oddball(int *arr, int len) {
	//set up true sum.                                        
	int xor_sum = 0;
	int i;                                              
                                                                  
	//add up to get the sum                                     
	for (i = len - 1; i >= 0; i--) {                               
		xor_sum ^= arr[i];                               
	}
	
	return xor_sum;
}

#endif

/**
 * Function: randGenerator
 * Description: Generate a random array that is in compliance with
 *   lab specification
 * Arguments:
 *   arr - pointer to start of integer array
 *   len - number of elements in array
 * Return:
 *   Unused, but feel free to utilize it.
 */

int randGenerator(int *arr, int len) {
	int i, j, r, rcount;
	for (i = 0; i < len; i++) {
		do {
			rcount = 0;
			r = rand()%(len/2 + 1) + 1;
			for (j = 0; j < i && rcount < 2; j++) {
				if (arr[j] == r) {
					rcount++;
				}
			}
		} while(rcount >= 2);
		arr[i] = r;
		printf("%d ", r);
	}
	printf("\nDone generating\n");
	return 0;
}



