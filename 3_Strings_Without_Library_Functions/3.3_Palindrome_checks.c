#include <stdio.h>
#include <stdint.h>

typedef enum{
PALIN_FAIL=0,
PALIN_PASS,
PALIN_NULL
}palin_status_t;

palin_status_t check_palindrome(const char* str)
{
    if(str==NULL)
        return PALIN_NULL;

    if (*str == '\0') {
        return PALIN_PASS; // Empty string is already reversed
    }

    const char* start = str;
    const char* end = str;

    while(*end != '\0')
        end++;

    end--;

    while(start<end)
    {
        if(*start!=*end)
            return PALIN_FAIL;
        start++;
        end--;
    }

    return PALIN_PASS;
}

#ifndef TESTING
int main()
{
    char a[10] = {'r','a','d','a','r','\0'};
    printf("%d\n",check_palindrome(a));
    return 0;
}
#endif