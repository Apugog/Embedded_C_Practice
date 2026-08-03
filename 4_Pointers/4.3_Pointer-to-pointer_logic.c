#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum{
    MEM_SUCCESS=0,
    MEM_NULL_PTR,
    MEM_ZERO_COUNT,
    MEM_ALLOC_FAIL
}mem_status_t;

mem_status_t alloc_buffer(uint32_t** pp_buf, size_t count, uint32_t init_val){
    if(pp_buf==NULL){
        return MEM_NULL_PTR;
    }

    if(count==0){
        return MEM_ZERO_COUNT;
    }

    if (count > SIZE_MAX / sizeof(uint32_t)) {
        return MEM_ALLOC_FAIL;
    }

    *pp_buf = malloc(sizeof(uint32_t)*count);
    if(*pp_buf==NULL){
        return MEM_ALLOC_FAIL;
    }

    uint32_t* temp = *pp_buf;
    for(size_t i=0; i<count; i++){
        (*temp)=init_val;
        temp=temp+1;
    }

    return MEM_SUCCESS;
}

mem_status_t free_buffer(uint32_t** pp_buf){
    if(pp_buf==NULL || *pp_buf==NULL){
        return MEM_NULL_PTR;
    }

    free(*pp_buf);
    *pp_buf = NULL;

    return MEM_SUCCESS;
}

#ifndef TESTING
int main(){

    uint32_t* buff;
    alloc_buffer(&buff, 10, 2);
    for(size_t i=0; i<10; i++){
        printf("%d,",buff[i]);
    }
    free_buffer(&buff);
    return 0;
}
#endif