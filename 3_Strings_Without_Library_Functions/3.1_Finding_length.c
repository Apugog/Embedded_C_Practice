#include <stdio.h>
#include <stdint.h>

size_t cus_strlen(const char* str)
{
    if(str == NULL)
        return 0;

    const char* s = str;
    size_t i = 0;
    while(*s!='\0')
    {
        s++;
        i++;
    }

    return i;
}

#ifndef TESTING
void main()
{
    char a[10] = {'H','e','l','l','o','\0'};
    printf("size = %zu\n",cus_strlen(a));
}
#endif