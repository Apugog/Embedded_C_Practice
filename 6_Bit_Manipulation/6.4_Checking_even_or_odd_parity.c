#include <stdio.h>
#include <stdint.h>

/**
 * @brief Parity indicator enum.
 * PARITY_EVEN (0): Even number of set bits (modulo-2 XOR sum is 0).
 * PARITY_ODD  (1): Odd number of set bits  (modulo-2 XOR sum is 1).
 */
typedef enum {
    PARITY_EVEN = 0,
    PARITY_ODD  = 1
} parity_t;

/**
 * @brief Computes the parity of a 32-bit unsigned integer in O(1) constant time.
 *
 * METHOD: Parallel XOR Tree-Folding + Nibble Magic Constant Lookup (0x6996)
 *
 * 1. Parallel Tree Folding:
 *    - Parity of a word is the horizontal XOR sum of all bits: b0 ^ b1 ^ ... ^ b31.
 *    - XOR is associative and commutative, so bits can be folded in parallel:
 *        val ^= (val >> 16U); // Folds 32 bits into 16 bits (16 XORs in 1 cycle)
 *        val ^= (val >> 8U);  // Folds 16 bits into 8 bits  (8 XORs in 1 cycle)
 *        val ^= (val >> 4U);  // Folds 8 bits into 4 bits   (4 XORs in 1 cycle)
 *    - After these 3 folds, the lower 4 bits (bits [3:0]) hold the XOR reduction
 *      of all 32 original bits.
 *
 * 2. Magic Constant Lookup (0x6996U):
 *    - A 4-bit nibble has 16 possible values (0x0 to 0xF).
 *    - 0x6996 in binary is: 0110 1001 1001 0110 (16 bits).
 *    - Notice that bit n of 0x6996 is exactly the parity of integer n:
 *        n=0  (0000) -> 0 ones -> parity 0 (bit 0 is 0)
 *        n=1  (0001) -> 1 one  -> parity 1 (bit 1 is 1)
 *        ...
 *        n=15 (1111) -> 4 ones -> parity 0 (bit 15 is 0)
 *    - By masking `(val & 0x0FU)`, we safely isolate the 4-bit folded value (0..15).
 *    - Shifting `(0x6996U >> (val & 0x0FU)) & 1U` retrieves the parity directly
 *      from a CPU register in 1 operation without any Flash/RAM array access.
 *
 * Performance Characteristics:
 * - Time Complexity: O(1) (deterministic, zero loops, zero branches).
 * - Space Complexity: O(1) (zero Flash/RAM table overhead).
 * - Hard real-time & side-channel attack safe.
 *
 * @param val 32-bit unsigned integer to evaluate.
 * @return parity_t PARITY_EVEN (0) or PARITY_ODD (1).
 */
parity_t check_parity(uint32_t val) {
    val ^= (val >> 16U);
    val ^= (val >> 8U);
    val ^= (val >> 4U);
    return (parity_t)((0x6996U >> (val & 0x0FU)) & 1U);
}

#ifndef TESTING
int main(void) {
    printf("val = 0          -> parity: %u (Expected: 0 - EVEN)\n", check_parity(0U));
    printf("val = 1          -> parity: %u (Expected: 1 - ODD)\n",  check_parity(1U));
    printf("val = 2          -> parity: %u (Expected: 1 - ODD)\n",  check_parity(2U));
    printf("val = 3          -> parity: %u (Expected: 0 - EVEN)\n", check_parity(3U));
    printf("val = 0x80000001 -> parity: %u (Expected: 0 - EVEN)\n", check_parity(0x80000001U));
    printf("val = 0xFFFFFFFF -> parity: %u (Expected: 0 - EVEN)\n", check_parity(0xFFFFFFFFU));
    return 0;
}
#endif
