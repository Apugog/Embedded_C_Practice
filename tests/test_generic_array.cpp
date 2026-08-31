#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>
#include <vector>

extern "C" {
    typedef enum {
        GEN_STATUS_SUCCESS = 0,
        GEN_STATUS_NULL_PTR,
        GEN_STATUS_EMPTY,
        GEN_STATUS_INVALID_SIZE
    } gen_status_t;

    typedef void (*elem_print_fn)(const void *elem);

    typedef struct {
        uint8_t id;
        int16_t temp;
    } sensor_t;

    void print_uint32(const void *elem);
    void print_float(const void *elem);
    void print_sensor(const void *elem);

    gen_status_t print_generic_array(const void *base,
                                    size_t num_elem,
                                    size_t elem_size,
                                    elem_print_fn print_cb);
}

// Global tracking for mock callbacks in tests
static std::vector<uint32_t> g_visited_u32;
static void mock_track_u32(const void *elem) {
    g_visited_u32.push_back(*(const uint32_t *)elem);
}

static std::vector<int16_t> g_visited_temps;
static void mock_track_sensor(const void *elem) {
    const sensor_t *s = (const sensor_t *)elem;
    g_visited_temps.push_back(s->temp);
}

class GenericArrayTest : public ::testing::Test {
protected:
    void SetUp() override {
        g_visited_u32.clear();
        g_visited_temps.clear();
    }
};

TEST_F(GenericArrayTest, NullBaseReturnsNullPtr) {
    uint32_t dummy = 10;
    EXPECT_EQ(print_generic_array(NULL, 5, sizeof(uint32_t), print_uint32), GEN_STATUS_NULL_PTR);
    EXPECT_EQ(print_generic_array(&dummy, 5, sizeof(uint32_t), NULL), GEN_STATUS_NULL_PTR);
    EXPECT_EQ(print_generic_array(NULL, 0, 0, NULL), GEN_STATUS_NULL_PTR);
}

TEST_F(GenericArrayTest, ZeroNumElemReturnsEmpty) {
    uint32_t arr[3] = {1, 2, 3};
    EXPECT_EQ(print_generic_array(arr, 0, sizeof(uint32_t), print_uint32), GEN_STATUS_EMPTY);
}

TEST_F(GenericArrayTest, ZeroElemSizeReturnsInvalidSize) {
    uint32_t arr[3] = {1, 2, 3};
    EXPECT_EQ(print_generic_array(arr, 3, 0, print_uint32), GEN_STATUS_INVALID_SIZE);
}

TEST_F(GenericArrayTest, Uint32ArrayTraversalAndCallback) {
    uint32_t arr[5] = {100, 200, 300, 400, 500};
    gen_status_t status = print_generic_array(arr, 5, sizeof(uint32_t), mock_track_u32);
    
    EXPECT_EQ(status, GEN_STATUS_SUCCESS);
    ASSERT_EQ(g_visited_u32.size(), 5u);
    for (size_t i = 0; i < 5; i++) {
        EXPECT_EQ(g_visited_u32[i], arr[i]);
    }
}

TEST_F(GenericArrayTest, FloatArraySuccess) {
    float arr[3] = {1.23f, 4.56f, 7.89f};
    EXPECT_EQ(print_generic_array(arr, 3, sizeof(float), print_float), GEN_STATUS_SUCCESS);
}

TEST_F(GenericArrayTest, StructArrayTraversalAndCallback) {
    sensor_t sensors[3] = {
        {.id = 1, .temp = -10},
        {.id = 2, .temp = 25},
        {.id = 3, .temp = 80}
    };
    
    gen_status_t status = print_generic_array(sensors, 3, sizeof(sensor_t), mock_track_sensor);
    
    EXPECT_EQ(status, GEN_STATUS_SUCCESS);
    ASSERT_EQ(g_visited_temps.size(), 3u);
    EXPECT_EQ(g_visited_temps[0], -10);
    EXPECT_EQ(g_visited_temps[1], 25);
    EXPECT_EQ(g_visited_temps[2], 80);
}

TEST_F(GenericArrayTest, SingleElement) {
    uint32_t single[1] = {9999};
    EXPECT_EQ(print_generic_array(single, 1, sizeof(uint32_t), mock_track_u32), GEN_STATUS_SUCCESS);
    ASSERT_EQ(g_visited_u32.size(), 1u);
    EXPECT_EQ(g_visited_u32[0], 9999u);
}
