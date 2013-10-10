#include <stdio.h>
#include "math.h"
#include <stdlib.h>
int errorno;

int main() {
	int a,b;
	long long int al,bl;
	char c;
	// using a global viarable to keep track of error.
	errorno =0;
	int num = 0;
	char* str = malloc(10000);
	char* strtmp = malloc(10000);
	while(fgets(str, 10000, stdin)) {
		 //read expression from stdin

		// scan the string into two operands as both int and long long
		// if the input operands are not valid digit, exit the program.
		if(sscanf(str,"%d %c %d%s",&a,&c,&b,strtmp) != 3) {
			printf("Invalid input!\n");
			return 0;
		}
		sscanf(str,"%lld %c %lld",&al,&c,&bl);		

		// If any integer is beyond an int scope or even a double scope,
		// it's int and long value should be different.
		if(al != a || bl != b){
			printf("parameter overflow!\n");
			errorno = 0;
			continue;
		}
		// If input operands are both valid, then start calculation
		// if the operator is not valid, exit the program.
		switch(c) {
			case '+':
				num =  add(a,b);
				break;
			case '-':
				num =  sub(a,b);
				break;
			case '*':
				num =  mul(a,b);
				break;
			case '/':
				num =  cdiv(a,b);
				break;
			case '%':
				num =  mod(a,b);
				break;
			default:
				printf("Invalid input!\n");
				return 0;
		}

		// If there is an error during calculation,
		// reset errorno to zero and print nothing
		if(errorno != 0){
			errorno = 0;
			continue;
		}
		else { // if there is no error, print result.
			printf("%d\n\n", num);
		}
	}
	return 0;
}
