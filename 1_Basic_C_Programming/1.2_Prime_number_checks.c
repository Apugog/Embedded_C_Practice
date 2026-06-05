#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

bool isPrime(uint32_t n)
{
    if(n <= 1)
        return false;

    if(n <=3)
        return true;

    if(n%2==0 || n%3==0)
        return false;
    
    for(uint32_t i = 5; i <n / i; i+=6)
    {
        if(n%i==0 || n%(i+2)==0)
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