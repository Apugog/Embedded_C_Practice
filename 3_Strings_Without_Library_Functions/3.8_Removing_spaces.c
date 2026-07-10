#include <stdio.h>
#include <stdint.h>

typedef enum{
    SPACE_NULL=0,
    SPACE_SUCCESS
} space_status_t;

space_status_t remove_space(char* str)
{
    if(str==NULL)
        return SPACE_NULL;

    char* l = str;
    char* r = str;

    while(*r!='\0')
    {
        if(*r!=' '){
            /* we could do swap but since \0
               is inserted it doesnot matter much */ 
            if(l!=r){
                *l = *r;
            }
            l++;
        }
        r++;
    }
    *l='\0';

    return SPACE_SUCCESS;
}

#ifndef TESTING
int main()
{
    char a[10] = {'H','e',' ','l','l',' ',' ','o','\0'};
    printf("%s\n",a);
    remove_space(a);
    printf("%s\n",a);

    return 0;
}
#endif 