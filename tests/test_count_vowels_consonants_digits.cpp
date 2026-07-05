#include <gtest/gtest.h>
#include <stddef.h>

extern "C" {
    typedef enum {
        COUNT_SUCCESS = 0,
        COUNT_NULL_PTR
    } count_status_t;

    typedef struct {
        size_t vowels;
        size_t consonants;
        size_t digits;
    } char_counts_t;

    count_status_t count_vowels_consonants_digits(const char* str, char_counts_t* counts);
}

TEST(CountVowelsConsonantsDigitsTests, HandlesNullString) {
    char_counts_t counts = {1, 2, 3}; // Prefill with garbage
    EXPECT_EQ(count_vowels_consonants_digits(NULL, &counts), COUNT_NULL_PTR);
    // Counts should remain unchanged or we don't care, but null ptr was returned
}

TEST(CountVowelsConsonantsDigitsTests, HandlesNullCountsStruct) {
    EXPECT_EQ(count_vowels_consonants_digits("test", NULL), COUNT_NULL_PTR);
}

TEST(CountVowelsConsonantsDigitsTests, HandlesEmptyString) {
    char_counts_t counts;
    EXPECT_EQ(count_vowels_consonants_digits("", &counts), COUNT_SUCCESS);
    EXPECT_EQ(counts.vowels, 0);
    EXPECT_EQ(counts.consonants, 0);
    EXPECT_EQ(counts.digits, 0);
}

TEST(CountVowelsConsonantsDigitsTests, CountsOnlyVowels) {
    char_counts_t counts;
    EXPECT_EQ(count_vowels_consonants_digits("aeiouAEIOU", &counts), COUNT_SUCCESS);
    EXPECT_EQ(counts.vowels, 10);
    EXPECT_EQ(counts.consonants, 0);
    EXPECT_EQ(counts.digits, 0);
}

TEST(CountVowelsConsonantsDigitsTests, CountsOnlyConsonants) {
    char_counts_t counts;
    EXPECT_EQ(count_vowels_consonants_digits("bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ", &counts), COUNT_SUCCESS);
    EXPECT_EQ(counts.vowels, 0);
    EXPECT_EQ(counts.consonants, 42);
    EXPECT_EQ(counts.digits, 0);
}

TEST(CountVowelsConsonantsDigitsTests, CountsOnlyDigits) {
    char_counts_t counts;
    EXPECT_EQ(count_vowels_consonants_digits("0123456789", &counts), COUNT_SUCCESS);
    EXPECT_EQ(counts.vowels, 0);
    EXPECT_EQ(counts.consonants, 0);
    EXPECT_EQ(counts.digits, 10);
}

TEST(CountVowelsConsonantsDigitsTests, IgnoresSpecialCharacters) {
    char_counts_t counts;
    EXPECT_EQ(count_vowels_consonants_digits("!@#$%^&*()_+=-`~ []\\{}|;':\",./<>?", &counts), COUNT_SUCCESS);
    EXPECT_EQ(counts.vowels, 0);
    EXPECT_EQ(counts.consonants, 0);
    EXPECT_EQ(counts.digits, 0);
}

TEST(CountVowelsConsonantsDigitsTests, MixedString) {
    char_counts_t counts;
    EXPECT_EQ(count_vowels_consonants_digits("Embedded Systems 2026!", &counts), COUNT_SUCCESS);
    // E-m-b-e-d-d-e-d  S-y-s-t-e-m-s  2-0-2-6-!
    // Vowels: E, e, e, e (4) -> Wait, is 'y' a vowel or consonant?
    // In English, 'y' is historically a consonant or semi-vowel, but in standard lists:
    // A, E, I, O, U / a, e, i, o, u.
    // So 'y' is counted as a consonant under our A,E,I,O,U rules.
    // Let's verify vowels: E (1), e (2), e (3), e (4). Total = 5. Wait:
    // E - m - b - e - d - d - e - d (vowels: E, e, e -> 3)
    // S - y - s - t - e - m - s (vowels: e -> 1)
    // Total vowels = 4 (E, e, e, e).
    // Let's verify consonants:
    // m, b, d, d, d (5)
    // S, y, s, t, m, s (6)
    // Total consonants = 11.
    // Let's verify digits: 2, 0, 2, 6 (4).
    EXPECT_EQ(counts.vowels, 4);
    EXPECT_EQ(counts.consonants, 11);
    EXPECT_EQ(counts.digits, 4);
}
