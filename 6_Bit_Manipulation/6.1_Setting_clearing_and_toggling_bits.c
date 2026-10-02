#include <stdio.h>
#include <stdint.h>

typedef enum{
    BIT_STATUS_OK,
    BIT_STATUS_NULL_PTR,
    BIT_STATUS_INVALID_BIT
} bit_status_t; 

bit_status_t bit_set(uint32_t *reg, uint8_t bit){
    if(reg==NULL){
        return BIT_STATUS_NULL_PTR;
    }

    if(bit>31){
        return BIT_STATUS_INVALID_BIT;
    }

    *reg |= (1U<<bit);

    return BIT_STATUS_OK;
}

bit_status_t bit_clear(uint32_t *reg, uint8_t bit){
    if(reg==NULL){
        return BIT_STATUS_NULL_PTR;
    }

    if(bit>31){
        return BIT_STATUS_INVALID_BIT;
    }

    *reg &= ~(1U<<bit);

    return BIT_STATUS_OK;
}

bit_status_t bit_toggle(uint32_t *reg, uint8_t bit){
    if(reg==NULL){
        return BIT_STATUS_NULL_PTR;
    }

    if(bit>31){
        return BIT_STATUS_INVALID_BIT;
    }

    *reg ^= (1U<<bit);

    return BIT_STATUS_OK;
}

#ifndef TESTING
int main(){
    uint32_t reg = 0x0;
    bit_set(&reg,2);
    printf("\n0x%08X",reg);
    bit_clear(&reg,2);
    printf("\n0x%08X",reg);
    bit_toggle(&reg,0);
    printf("\n0x%08X",reg);
    bit_toggle(&reg,0);
    printf("\n0x%08X",reg);

    return 0;
}
#endif