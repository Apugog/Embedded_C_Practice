#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

extern "C" {
    typedef enum {
        SWAP_NULL = 0,
        SWAP_SUCCESS
    } swap_status_t;

    swap_status_t swap_ptrs(void* a, void* b);
}

TEST(SwapPtrsTest, HandlesStandardSwap) {
    uint32_t a = 10;
    uint32_t b = 20;
    uint32_t* ptr_a = &a;
    uint32_t* ptr_b = &b;

    EXPECT_EQ(swap_ptrs(&ptr_a, &ptr_b), SWAP_SUCCESS);
    EXPECT_EQ(ptr_a, &b);
    EXPECT_EQ(ptr_b, &a);
}

TEST(SwapPtrsTest, HandlesNullPointers) {
    uint32_t a = 10;
    uint32_t* ptr_a = &a;

    EXPECT_EQ(swap_ptrs(&ptr_a, NULL), SWAP_NULL);
    EXPECT_EQ(swap_ptrs(NULL, &ptr_a), SWAP_NULL);
    EXPECT_EQ(swap_ptrs(NULL, NULL), SWAP_NULL);
    
    // Ensure ptr_a was not modified
    EXPECT_EQ(ptr_a, &a);
}

TEST(SwapPtrsTest, HandlesAliasing) {
    uint32_t a = 10;
    uint32_t* ptr_a = &a;

    EXPECT_EQ(swap_ptrs(&ptr_a, &ptr_a), SWAP_SUCCESS);
    EXPECT_EQ(ptr_a, &a);
}

TEST(SwapPtrsTest, HandlesDifferentPointerTypes) {
    uint32_t a = 10;
    double b = 20.0;
    uint32_t* ptr_a = &a;
    double* ptr_b = &b;

    EXPECT_EQ(swap_ptrs(&ptr_a, &ptr_b), SWAP_SUCCESS);
    EXPECT_EQ((double*)ptr_a, &b);
    EXPECT_EQ((uint32_t*)ptr_b, &a);
}
