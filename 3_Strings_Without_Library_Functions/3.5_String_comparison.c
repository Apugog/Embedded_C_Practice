#include <stdio.h>
#include <stdint.h>

typedef enum{
    STRCMP_SUCCESS,
    STRCMP_NULL_PTR
}strcmp_status_t;

strcmp_status_t custom_setup(const char* s1, const char* s2, int32_t* result)
{
    const unsigned char* a = (const unsigned char*)s1;
    const unsigned char* b = (const unsigned char*)s2;
    

    if(s1==NULL || s2==NULL || result==NULL)
        return STRCMP_NULL_PTR;

    *result = 0;

    while(*a!='\0' || *b!='\0'){
        if(*a!=*b){
            break;
        }
        a++;
        b++;
    }

    if(*a<*b){
        *result = -1;
    }
    else if(*a>*b){
        *result = 1;
    }
    else{
        *result = 0;
    }

    return STRCMP_SUCCESS;
}

#ifndef TESTING
int main()
{
    int32_t result = 0;
    custom_setup("cat","Cat",&result);
    printf("cmp res =%d",result);
    return 0;
}
#endif 