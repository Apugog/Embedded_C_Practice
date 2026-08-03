#include <gtest/gtest.h>
#include <stdint.h>

extern "C" {
    typedef struct LRUCache LRUCache;

    LRUCache* lru_cache_create(int32_t capacity);
    int32_t lru_cache_get(LRUCache* cache, int32_t key);
    void lru_cache_put(LRUCache* cache, int32_t key, int32_t value);
    void lru_cache_free(LRUCache* cache);
}

TEST(LRUCacheTests, BasicOperations) {
    LRUCache* cache = lru_cache_create(2);
    ASSERT_NE(cache, nullptr);

    // Initial state
    EXPECT_EQ(lru_cache_get(cache, 1), -1);

    // Put some values
    lru_cache_put(cache, 1, 10);
    lru_cache_put(cache, 2, 20);
    EXPECT_EQ(lru_cache_get(cache, 1), 10);
    EXPECT_EQ(lru_cache_get(cache, 2), 20);

    // Put dynamic eviction check
    lru_cache_put(cache, 3, 30); // Evicts key 1 (since key 1 was got before 2, wait, key 1 was accessed after key 2 was put. So key 2 is LRU!)
    // Access order:
    // put(1, 10) -> cache: [1:10]
    // put(2, 20) -> cache: [2:20, 1:10]
    // get(1)     -> cache: [1:10, 2:20]
    // get(2)     -> cache: [2:20, 1:10]
    // put(3, 30) -> evicts 1 (since 2 was most recently accessed)
    
    // Wait, let's verify standard eviction.
    // Let's reset cache and do a simpler trace.
    lru_cache_free(cache);
}

TEST(LRUCacheTests, EvictionSequence) {
    LRUCache* cache = lru_cache_create(2);
    ASSERT_NE(cache, nullptr);

    lru_cache_put(cache, 1, 10);
    lru_cache_put(cache, 2, 20);
    
    // Get 1 makes key 1 MRU (Most Recently Used). Cache order (MRU -> LRU): [1, 2]
    EXPECT_EQ(lru_cache_get(cache, 1), 10);
    
    // Put 3 evicts the LRU key (key 2). Cache order (MRU -> LRU): [3, 1]
    lru_cache_put(cache, 3, 30);
    
    EXPECT_EQ(lru_cache_get(cache, 2), -1); // Evicted
    EXPECT_EQ(lru_cache_get(cache, 1), 10); // Still exists
    
    // Put 4 evicts key 3 (since key 1 was just accessed via get). Cache order: [4, 1]
    lru_cache_put(cache, 4, 40);
    EXPECT_EQ(lru_cache_get(cache, 3), -1); // Evicted
    EXPECT_EQ(lru_cache_get(cache, 1), 10); // Still exists
    EXPECT_EQ(lru_cache_get(cache, 4), 40); // Exists

    lru_cache_free(cache);
}

TEST(LRUCacheTests, HandlesNullCache) {
    // Basic defensive checks
    EXPECT_EQ(lru_cache_get(nullptr, 1), -1);
    lru_cache_put(nullptr, 1, 10); // Should not crash
    lru_cache_free(nullptr);        // Should not crash
}

TEST(LRUCacheTests, ZeroOrNegativeCapacity) {
    LRUCache* cache = lru_cache_create(0);
    EXPECT_EQ(cache, nullptr);
    
    cache = lru_cache_create(-5);
    EXPECT_EQ(cache, nullptr);
}
