#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

extern "C" {
    typedef enum {
        MEM_SUCCESS = 0,
        MEM_NULL_PTR,
        MEM_ZERO_COUNT,
        MEM_ALLOC_FAIL
    } mem_status_t;

    mem_status_t alloc_buffer(uint32_t** pp_buf, size_t count, uint32_t init_val);
    mem_status_t free_buffer(uint32_t** pp_buf);
}

TEST(PtrToPtrTest, AllocSuccess) {
    uint32_t* buff = NULL;
    EXPECT_EQ(alloc_buffer(&buff, 5, 42), MEM_SUCCESS);
    ASSERT_NE(buff, (uint32_t*)NULL);
    for (size_t i = 0; i < 5; i++) {
        EXPECT_EQ(buff[i], 42u);
    }
    EXPECT_EQ(free_buffer(&buff), MEM_SUCCESS);
    EXPECT_EQ(buff, (uint32_t*)NULL);
}

TEST(PtrToPtrTest, AllocNullDoublePointer) {
    EXPECT_EQ(alloc_buffer(NULL, 10, 5), MEM_NULL_PTR);
}

TEST(PtrToPtrTest, AllocZeroCount) {
    uint32_t* buff = NULL;
    EXPECT_EQ(alloc_buffer(&buff, 0, 5), MEM_ZERO_COUNT);
    EXPECT_EQ(buff, (uint32_t*)NULL);
}

TEST(PtrToPtrTest, AllocOverflowCount) {
    uint32_t* buff = NULL;
    size_t invalid_count = SIZE_MAX / sizeof(uint32_t) + 1;
    EXPECT_EQ(alloc_buffer(&buff, invalid_count, 5), MEM_ALLOC_FAIL);
    EXPECT_EQ(buff, (uint32_t*)NULL);
}

TEST(PtrToPtrTest, FreeNullPointer) {
    uint32_t* buff = NULL;
    EXPECT_EQ(free_buffer(NULL), MEM_NULL_PTR);
    EXPECT_EQ(free_buffer(&buff), MEM_NULL_PTR);
}
