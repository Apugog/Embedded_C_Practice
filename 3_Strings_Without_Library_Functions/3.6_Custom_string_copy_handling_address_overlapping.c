#include <stdio.h>
#include <stdint.h>

typedef enum{
    STRCPY_SUCCESS=0,
    STRCPY_NULL_PTR
}strcpy_status_t;

strcpy_status_t custom_strcpy(char* dest, const char* src)
{
   if(dest==NULL || src==NULL)
        return STRCPY_NULL_PTR;

    const char* src_end = src;
    size_t len = 0;
    while(*src_end!='\0'){
        src_end++;
        len++;
    }
        
    if(src <= dest && dest <= src_end)
    {
        // reverse copy
        for (size_t i = len+1; i>0;i--){
            dest[i-1] = src[i-1];
        }
    }
    else
    {
        // forward copy
        for(size_t i = 0; i<=len;i++){
            dest[i]=src[i];
        }
    }


   return STRCPY_SUCCESS;
}

#ifndef TESTING
int main()
{

    char a[10]="Pramit";
    char b[10]="Hola";

    printf("%s \n",a);
    custom_strcpy(a+2,b);
    printf("%s \n",a);

    return 0;
}
#endif