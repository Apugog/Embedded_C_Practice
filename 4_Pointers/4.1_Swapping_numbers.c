#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

typedef enum{
    SWAP_NULL=0,
    SWAP_SUCCESS
}swap_status_t;

swap_status_t swap_ptrs(void* a, void* b)
{

    if(a==NULL || b==NULL){
        return SWAP_NULL;
    }

    if(a == b){
        return SWAP_SUCCESS;
    }

    /* Cast to unsigned char* to access memory byte-by-byte.
     * Character types are a special exception in C and are allowed to 
     * alias any object type, avoiding strict aliasing violations. */
    unsigned char* p1 = (unsigned char*)a;
    unsigned char* p2 = (unsigned char*)b;

    /* Swap the bytes of the pointers (each pointer is sizeof(void*) bytes) */
    for (size_t i = 0; i < sizeof(void*); i++) {
        unsigned char temp = p1[i];
        p1[i] = p2[i];
        p2[i] = temp;
    }

    return SWAP_SUCCESS;
}

#ifndef TESTING
int main()
{
    uint32_t a = 10;
    uint32_t b = 20;
    
    uint32_t* ptr_a= &a;
    uint32_t* ptr_b= &b;

    printf("%d %d\n",*ptr_a,*ptr_b);
    swap_ptrs(&ptr_a, &ptr_b);
    printf("%d %d\n",*ptr_a,*ptr_b);

    return 0;
}
#endif