#include <stdio.h>
#include <stdint.h>

typedef enum{
REV_SUCCESS=0,
REV_NULL
}rev_status_t;

rev_status_t reverse_string(char* str)
{
    if(str==NULL)
        return REV_NULL;

    if (*str == '\0') {
        return REV_SUCCESS; // Empty string is already reversed
    }

    char* start = str;
    char* end = str;

    while(*end != '\0')
        end++;

    end--;

    while(start<end)
    {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }

    return REV_SUCCESS;
}

#ifndef TESTING
int main()
{
    char a[10] = {'H','e','l','l','o','\0'};
    reverse_string(a);
    printf("%s",a);
    return 0;
}
#endif