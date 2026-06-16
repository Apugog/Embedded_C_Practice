// Implement your GCD function here.
// Function signature: uint32_t findGCD(int32_t a, int32_t b);
#include <stdio.h>
#include <stdint.h>

uint32_t findGCD(int32_t a, int32_t b)
{

    uint32_t ua = a<0 ? -(uint32_t)a:(uint32_t)a;
    uint32_t ub = b<0 ? -(uint32_t)b:(uint32_t)b;

    if(ua==0)
        return ub;
    if(ub==0)
        return ua;

    while(ub != 0)
    {
        uint32_t temp = ub;
        ub = ua % ub;
        ua = temp;
    }

    return ua;
}

#ifndef TESTING
int main()
{
    printf("%d", findGCD(18,12));
    return 0;
}
#endif