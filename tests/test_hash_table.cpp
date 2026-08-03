#include <gtest/gtest.h>
#include <stdint.h>

extern "C" {
    typedef struct HashTable HashTable;

    HashTable* hash_table_create(size_t capacity);
    bool hash_table_insert(HashTable* table, int32_t key, int32_t value);
    bool hash_table_get(const HashTable* table, int32_t key, int32_t* out_value);
    bool hash_table_remove(HashTable* table, int32_t key);
    void hash_table_free(HashTable* table);
}

TEST(HashTableTests, BasicOperations) {
    HashTable* table = hash_table_create(10);
    ASSERT_NE(table, nullptr);

    int32_t val = 0;
    EXPECT_FALSE(hash_table_get(table, 5, &val));

    // Insertion
    EXPECT_TRUE(hash_table_insert(table, 5, 50));
    EXPECT_TRUE(hash_table_get(table, 5, &val));
    EXPECT_EQ(val, 50);

    // Update existing key
    EXPECT_TRUE(hash_table_insert(table, 5, 100));
    EXPECT_TRUE(hash_table_get(table, 5, &val));
    EXPECT_EQ(val, 100);

    // Removal
    EXPECT_TRUE(hash_table_remove(table, 5));
    EXPECT_FALSE(hash_table_get(table, 5, &val));

    hash_table_free(table);
}

TEST(HashTableTests, CollisionHandling) {
    // Create a very small capacity to force collisions
    HashTable* table = hash_table_create(2);
    ASSERT_NE(table, nullptr);

    // Keys 2 and 4 will collide (both % 2 == 0)
    EXPECT_TRUE(hash_table_insert(table, 2, 20));
    EXPECT_TRUE(hash_table_insert(table, 4, 40));

    int32_t val = 0;
    EXPECT_TRUE(hash_table_get(table, 2, &val));
    EXPECT_EQ(val, 20);

    EXPECT_TRUE(hash_table_get(table, 4, &val));
    EXPECT_EQ(val, 40);

    // Remove one of the colliding elements
    EXPECT_TRUE(hash_table_remove(table, 2));
    EXPECT_FALSE(hash_table_get(table, 2, &val));
    EXPECT_TRUE(hash_table_get(table, 4, &val));
    EXPECT_EQ(val, 40);

    hash_table_free(table);
}

TEST(HashTableTests, NullChecks) {
    int32_t val = 0;
    EXPECT_FALSE(hash_table_insert(nullptr, 1, 10));
    EXPECT_FALSE(hash_table_get(nullptr, 1, &val));
    EXPECT_FALSE(hash_table_remove(nullptr, 1));
    hash_table_free(nullptr); // Should not crash
}

TEST(HashTableTests, ZeroCapacity) {
    HashTable* table = hash_table_create(0);
    EXPECT_EQ(table, nullptr);
}
