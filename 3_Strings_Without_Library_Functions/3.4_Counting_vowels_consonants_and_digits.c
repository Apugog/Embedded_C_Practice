#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

typedef enum {
    COUNT_SUCCESS = 0,
    COUNT_NULL_PTR
} count_status_t;

typedef struct {
    size_t vowels;
    size_t consonants;
    size_t digits;
} char_counts_t;

count_status_t count_vowels_consonants_digits(const char* str, char_counts_t* counts) {
    if (str == NULL || counts == NULL) {
        return COUNT_NULL_PTR;
    }

    // Initialize counts to zero to prevent garbage values if caller passes uninitialized struct
    counts->vowels = 0;
    counts->consonants = 0;
    counts->digits = 0;

    const char* c = str;
    while (*c != '\0') {
        if (*c >= '0' && *c <= '9') {
            counts->digits++;
        } else if ((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z')) {
            // Case insensitivity: bitwise OR with 0x20 converts uppercase to lowercase
            char lower = *c | 0x20;
            if (lower == 'a' || lower == 'e' || lower == 'i' || lower == 'o' || lower == 'u') {
                counts->vowels++;
            } else {
                counts->consonants++;
            }
        }
        c++;
    }

    return COUNT_SUCCESS;
}

#ifndef TESTING
int main() {
    char a[10] = {'H', 'e', '1', '2', 'o', '\0'};
    char_counts_t count = {0};
    if (count_vowels_consonants_digits(a, &count) == COUNT_SUCCESS) {
        printf("digits = %zu\n", count.digits);
        printf("vowels = %zu\n", count.vowels);
        printf("consonants = %zu\n", count.consonants);
    }
    return 0;
}
#endif