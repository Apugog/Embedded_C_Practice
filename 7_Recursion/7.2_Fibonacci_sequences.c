#include <stdio.h>
#include <stdint.h>

#define FIB_MAX_INPUT_32BIT (46)

static void fibonacci_help(uint32_t num, uint32_t current, uint32_t next) {
    if (num == 0) {
        return;
    }
    printf("%u%s", current, (num == 1) ? "\n" : ", ");
    fibonacci_help(num - 1, next, current + next);
}

void fibonacci(uint32_t num) {
    if (num == 0 || num > FIB_MAX_INPUT_32BIT) {
        printf("Error: Term count must be between 1 and %d\n", FIB_MAX_INPUT_32BIT);
        return;
    }
    fibonacci_help(num, 0, 1);
}

int32_t fibonacci_nth_recursive(int32_t n) {
    if (n < 0 || n > FIB_MAX_INPUT_32BIT) {
        return -1;
    }
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    return fibonacci_nth_recursive(n - 1) + fibonacci_nth_recursive(n - 2);
}

#ifndef TESTING
int main(void) {
    printf("--- Linear Tail-Recursive Series (First 10 terms) ---\n");
    fibonacci(10);

    printf("\n--- Classic Tree-Recursive nth Values ---\n");
    for (int32_t i = 0; i <= 10; i++) {
        printf("F(%d) = %d\n", i, fibonacci_nth_recursive(i));
    }

    return 0;
}
#endif
