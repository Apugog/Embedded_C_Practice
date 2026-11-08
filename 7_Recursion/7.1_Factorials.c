/*
BASE CONCEPT
------------
f(1) = 1
f(2) = 2* f(1)
f(3) = 3* f(2)
f(n) = n*f(n-1)
*/
#include <stdio.h>

#define FACT_MAX_INPUT_32BIT  (12)

int fact(int n){
    if (n < 0 || n > FACT_MAX_INPUT_32BIT) {
        return -1; // Error: invalid input or 32-bit overflow
    }

    if(n<=1){
        return 1;
    }

    return n * fact(n-1);
}

#ifndef TESTING
int main(void){
    printf("factorial of %d is %d\n", 5, fact(5));

    return 0;
}
#endif
