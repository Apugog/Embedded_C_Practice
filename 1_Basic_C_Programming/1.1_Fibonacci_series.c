#include <stdio.h>
#include <stdint.h>

/**
 * @brief Prints the first n Fibonacci numbers.
 * 
 * @param n Total number of terms to print.
 * @note  Calculations use uint32_t. Overflow occurs after the 47th term.
 *        Expert Level: Handles edge cases, prevents overflow, and uses 
 *        a single loop for clean logic.
 */
void print_fibonacci(uint8_t n)
{
    if (n == 0) 
    {
        printf("Count must be greater than 0.\n");
        return;
    }

    uint32_t a = 0;
    uint32_t b = 1;

    printf("Fibonacci (%u terms): ", n);

    for (uint8_t i = 0; i < n; i++)
    {
        // Print current term
        printf("%u%s", a, (i == n - 1) ? ".\n" : ", ");

        // Prepare next term with overflow protection
        if (i < n - 1) 
        {
            if (UINT32_MAX - a < b) 
            {
                printf("\n[Error] Overflow detected! Cannot calculate term %u.\n", i + 2);
                break;
            }
            uint32_t next = a + b;
            a = b;
            b = next;
        }
    }
}

#ifndef TESTING
int main(void)
{
    // Test typical case
    print_fibonacci(10);

    // Test edge case
    print_fibonacci(1);

    // Test overflow awareness
    print_fibonacci(50);

    return 0;
}
#endif