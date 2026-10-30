#include <stdio.h>
#include <stdint.h>

typedef enum{
    BIT_RNG_OK,
    BIT_RNG_NULL,
    BIT_RNG_INVALID_INPUT
} bit_rng_status_t;

bit_rng_status_t set_bit_range(uint32_t *value, size_t start, size_t end){
    if(value == NULL){
        return BIT_RNG_NULL;
    }

    if((start > 31) || (end > 31) || (end > start)){
        return BIT_RNG_INVALID_INPUT;
    }
    // (((1U << start) - 1) ^ ((1U << end-1)-1)); --> old idea
    uint32_t mask = (~0U >> (31U - start)) & ~((1U << end) - 1U); 
    *value |= mask; 

    return BIT_RNG_OK;
}

bit_rng_status_t clear_bit_range(uint32_t* value, size_t start, size_t end){
    if(value == NULL){
        return BIT_RNG_NULL;
    }

    if((start > 31) || (end > 31) || (end > start)){
        return BIT_RNG_INVALID_INPUT;
    }

    uint32_t mask = (~0U >> (31U - start)) & ~((1U << end) - 1U);
    *value &= ~mask;
    return BIT_RNG_OK; 
}

#ifndef TESTING
int  main(){

    uint32_t value = 0x0;
    printf("errorcode %d\n",set_bit_range(&value,8,7));
    printf("set bit value is 0x%x\n", value);
    printf("errorcode %d\n",clear_bit_range(&value,8,7));
    printf("clear bit value is 0x%x\n", value);
    return 0;
}
#endif