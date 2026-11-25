#include <stdio.h>
#include <stdint.h>

static uint32_t sum_of_digit_helper(uint32_t num, uint32_t sum){
    if(num == 0){
        return sum;
    }
    sum = sum + num%10;
    return sum_of_digit_helper(num/10, sum);
}

uint32_t sum_of_digit(uint32_t num){
    return sum_of_digit_helper(num, 0);    
}

#ifndef TESTING
int main(void){
    printf("%u\n", sum_of_digit(234));
    return 0;
}
#endif