#include <stdio.h>
#include <stdint.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

typedef enum{
    FREQ_SUCCESS=0,
    FREQ_NULL_PTR,
    FREQ_INVALID_INPUT
}freq_status_t;

freq_status_t count_frequency(const char* s, uint32_t* arr, size_t len)
{
    if(s==NULL || arr==NULL){
        return FREQ_NULL_PTR;
    }

    if(len<26){
        return FREQ_INVALID_INPUT;
    }

    for(size_t i=0;i<len;i++){
        arr[i] = 0;
    }

    for(size_t i=0;s[i]!='\0';i++)
    {
        if(s[i]<'a' || s[i]>'z'){
            return FREQ_INVALID_INPUT;
        }
        arr[s[i]-'a']++;
    }

    return FREQ_SUCCESS;
}

static void print_arr(uint32_t* arr, size_t len)
{
    for(size_t i=0; i<len; i++){
        printf("%d%s",arr[i],i!=len-1?",":".\n");
    }
}

#ifndef TESTING
int main()
{
    uint32_t arr[26]={0};
    print_arr(arr,ARRAY_SIZE(arr));
    count_frequency("amarnampramit",arr,ARRAY_SIZE(arr));
    print_arr(arr,ARRAY_SIZE(arr));
    return 0;
}
#endif