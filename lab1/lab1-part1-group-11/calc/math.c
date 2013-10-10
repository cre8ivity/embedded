#include "math.h"
#include <stdio.h>
#include <stdlib.h>
int errorno;

//add function
int add(int a, int b) {

	//printf("%d",a);
	int c = a+b;
	int is_positive_a = a & 0x80000000;
	int is_positive_b = b & 0x80000000;
	int is_positive_c = c & 0x80000000;

	//printf("%d %d %d\n",is_positive_a,is_positive_b,is_positive_c);
	
	
	if(is_positive_a == is_positive_b && is_positive_b != is_positive_c){
		printf("overflow\n");
		errorno = -1;
	}
	
    return c;
}

//minus function
int sub(int a, int b) {
	int c = a-b;
	int is_positive_a = a & 0x80000000;
	int is_positive_b = b & 0x80000000;
	int is_positive_c = c & 0x80000000;

 	if(is_positive_a != is_positive_b && is_positive_a !=is_positive_c)	{
		printf("overflow\n");
		errorno = -1;
	}

    return c;
}

//multiplication function
int mul(int a, int b) {

	//printf("a: %d\n", a);
	//printf("b: %d\n", b);

	int c = a*b;
	long long int c_long = (long long int)a*(long long int)b;

	if(c != c_long){
		printf("Overflow\n");
		errorno = -1;
	}
	return c;
    
}

//division function
int cdiv(int a, int b) {

	int result = 0;

	if(b == 0) {
		printf("Divisor cannot be zero\n");
		errorno = -1;
	}
	else {
		result = a/b;
	}
    	return result;
}

//modulo function
int mod(int a, int b) {

	int result=0;
	if(b == 0){
		printf("Modulus cannot be zero\n");
		errorno = -1;
	}
    else {
		result = a%b;
	}
    return result;
}
	
