#include <stdio.h>
#include <stdint.h>

uint8_t countNum(int32_t num)
{
    uint8_t count = 0;
    
    if (num == 0)
        return 1; 

    while (num)
    {
        num = num / 10;
        count++;
    }
    return count;
}

#ifndef TESTING
int main(void)
{
    printf("num digits: %u\n", countNum(0));
    printf("INT32_MIN digits: %u\n", countNum(INT32_MIN));
    return 0;
}
#endif