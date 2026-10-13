#include <stdio.h>
#include <stdint.h>

uint8_t count_bits(uint32_t value){
    uint8_t count=0;

    while(value != 0){
        value &= (value-1U);
        count++;
    }

    return count;
}

uint8_t count_bits_swar(uint32_t value) {

    value = value - ((value>>1U) & 0x55555555U);

    value = (value & 0x33333333U) + ((value>>2U) & 0x33333333U);

    value = (value + (value >> 4U)) & 0x0F0F0F0FU;

    return (uint8_t)((value * 0x01010101U) >> 24U);
}

#ifndef TESTING
int main(){
    uint32_t value = 0b10110001;
    printf("count : %u\n", count_bits(value));
    printf("swar count : %u\n", count_bits_swar(value));
    return 0;
}
#endif