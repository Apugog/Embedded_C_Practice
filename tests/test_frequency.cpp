#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

extern "C" {
    typedef enum {
        FREQ_SUCCESS = 0,
        FREQ_NULL_PTR,
        FREQ_INVALID_INPUT
    } freq_status_t;

    freq_status_t count_frequency(const char* s, uint32_t* arr, size_t len);
}

TEST(FrequencyTests, HandlesNullString) {
    uint32_t arr[26] = {0};
    EXPECT_EQ(count_frequency(NULL, arr, 26), FREQ_NULL_PTR);
}

TEST(FrequencyTests, HandlesNullArray) {
    EXPECT_EQ(count_frequency("abc", NULL, 26), FREQ_NULL_PTR);
}

TEST(FrequencyTests, HandlesInsufficientLength) {
    uint32_t arr[25] = {0};
    EXPECT_EQ(count_frequency("abc", arr, 25), FREQ_INVALID_INPUT);
}

TEST(FrequencyTests, HandlesInvalidCharacters) {
    uint32_t arr[26] = {0};
    EXPECT_EQ(count_frequency("abcA", arr, 26), FREQ_INVALID_INPUT);
    EXPECT_EQ(count_frequency("abc ", arr, 26), FREQ_INVALID_INPUT);
    EXPECT_EQ(count_frequency("abc1", arr, 26), FREQ_INVALID_INPUT);
}

TEST(FrequencyTests, EmptyString) {
    uint32_t arr[26] = {1, 2, 3}; // Prefill with non-zero to test initialization
    EXPECT_EQ(count_frequency("", arr, 26), FREQ_SUCCESS);
    for (int i = 0; i < 26; i++) {
        EXPECT_EQ(arr[i], 0);
    }
}

TEST(FrequencyTests, ValidStringCount) {
    uint32_t arr[26] = {0};
    EXPECT_EQ(count_frequency("amarnampramit", arr, 26), FREQ_SUCCESS);
    
    // "amarnampramit"
    // a: 4, m: 3, r: 2, n: 1, p: 1, i: 1, t: 1
    EXPECT_EQ(arr['a' - 'a'], 4);
    EXPECT_EQ(arr['m' - 'a'], 3);
    EXPECT_EQ(arr['r' - 'a'], 2);
    EXPECT_EQ(arr['n' - 'a'], 1);
    EXPECT_EQ(arr['p' - 'a'], 1);
    EXPECT_EQ(arr['i' - 'a'], 1);
    EXPECT_EQ(arr['t' - 'a'], 1);
    
    // Check others are 0
    for (int i = 0; i < 26; i++) {
        char c = 'a' + i;
        if (c != 'a' && c != 'm' && c != 'r' && c != 'n' && c != 'p' && c != 'i' && c != 't') {
            EXPECT_EQ(arr[i], 0);
        }
    }
}
