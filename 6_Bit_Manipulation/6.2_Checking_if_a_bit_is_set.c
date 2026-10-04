#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

bool bit_is_set(uint32_t reg, uint8_t bit){
    if (bit>31){
        return false;
    }
    return (((reg>>bit) & 1U) != 0U);
}

#ifndef TESTING
int main(){

    uint32_t reg = 0x4;
    printf("Bit 2 is %s\n", bit_is_set(reg,2)? "set":"not set");
    printf("Bit 1 is %s", bit_is_set(reg,1)? "set":"not set");
    return 0;
}
#endif