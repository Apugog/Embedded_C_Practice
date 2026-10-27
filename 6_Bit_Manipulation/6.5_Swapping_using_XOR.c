#include <stdio.h>
#include <stdint.h>

typedef enum{
    SWAP_OK,
    SWAP_NULL
} swap_status_t;

swap_status_t swap_xor(uint32_t *a, uint32_t *b){
    if(a == NULL || b==NULL){
        return SWAP_NULL;
    }

    if(a==b){
        return SWAP_OK;
    }

    *a = *a^*b;
    *b = *a^*b;
    *a = *a^*b;

    return SWAP_OK;
}

#ifndef TESTING
int main(){

    uint32_t a = 10;
    uint32_t b = 20;

    printf("Before swap: a=%u, b=%u\n", a, b);

    swap_xor(&a,&b);

    printf("After swap: a=%u, b=%u\n", a, b);

    return 0;
}
#endif