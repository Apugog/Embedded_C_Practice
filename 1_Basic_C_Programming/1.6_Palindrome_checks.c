// Implement your palindrome check here.
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

bool isPalindrome(int32_t num)
{
    if (num < 0 || (num%10 == 0 && num!=0))
        return false;

    int32_t part1 = num;
    int32_t part2 = 0;

    while(part1>part2)
    {
        part2 = part2*10 + part1%10;
        part1 = part1/10;
    }
    return (part1==part2) || (part1==part2/10);
}

#ifndef TESTING
int main()
{
    printf("%d", isPalindrome(101232101));
    return 0;
}
#endif