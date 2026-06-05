#include <stdio.h>
#include <stdint.h>

int8_t reverseNum(int32_t num, int32_t *result)
{
    int32_t r_num = 0;

    if (result == NULL)
        return -1;

    while(num)
    {
        int8_t digit = num % 10;
        if(r_num > INT32_MAX/10 || (r_num == INT32_MAX/10 && digit > 7))
        {
            return -1;
        }

        if(r_num < INT32_MIN/10 || (r_num == INT32_MIN/10 && digit < -8))
        {
            return -1;
        }
        r_num = r_num * 10 + digit;
        num = num /10;
    }
    *result = r_num;
    return 0;
}

#ifndef TESTING
int main()
{
    int32_t result = 0;
    printf("%s\n",reverseNum(1000000005, &result)>=0?"PASS":"FAIL");
    printf("%d \n", result);
    return 0;
}
#endif