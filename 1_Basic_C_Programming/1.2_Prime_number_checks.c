#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

bool isPrime(uint32_t n)
{
    if(n==0 || n==1)
        return false;

    if(n==2)
        return true;
    
    for(uint32_t i = 2; i<n-1;i++)
    {
        if(n%i==0)
            return false;
    }

    return true;
}

#ifndef TESTING
int main()
{
    printf("%u ",isPrime(10));
    printf("%u ",isPrime(2));
    return 0;
}
#endif