#include <stdio.h>
#include <stdint.h>

void swapNum(int32_t* a, int32_t* b)
{
    if(a == NULL || b == NULL || a==b)
        return;
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

#ifndef TESTING
int main()
{
    int32_t a = 31;
    int32_t b = 10;

    printf("%d %d\n", a, b);
    swapNum(&a, &b);
    printf("swaping ..\n");
    printf("%d %d\n", a, b);

    return 0;
}
#endif