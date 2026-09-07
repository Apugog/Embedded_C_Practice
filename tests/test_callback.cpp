#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>
#include <vector>

extern "C" {
    typedef enum {
        STATUS_SUCCESS,
        STATUS_NULL_PTR,
        STATUS_INVALID_PARAM
    } status_t;

    typedef struct {
        uint32_t alarm_count;
        uint8_t max_val;
    } monitor_context_t;

    typedef void (*threshold_cb)(uint8_t val, size_t index, void *user_data);

    void pr_over_temp(uint8_t val, size_t index, void *user_data);
    status_t sensor_mon(const uint8_t *arr, size_t len, uint8_t threshold, threshold_cb call_back, void *user_data);
}

// Struct to record callback invocations for testing
struct BreachedEvent {
    uint8_t val;
    size_t index;
};

static void test_mock_callback(uint8_t val, size_t index, void *user_data) {
    if (user_data != NULL) {
        auto *events = static_cast<std::vector<BreachedEvent> *>(user_data);
        events->push_back({val, index});
    }
}

class CallbackTest : public ::testing::Test {
protected:
    std::vector<BreachedEvent> events;

    void SetUp() override {
        events.clear();
    }
};

TEST_F(CallbackTest, NullArrayReturnsNullPtr) {
    EXPECT_EQ(sensor_mon(NULL, 5, 10, test_mock_callback, &events), STATUS_NULL_PTR);
}

TEST_F(CallbackTest, NullCallbackReturnsNullPtr) {
    uint8_t arr[3] = {1, 2, 3};
    EXPECT_EQ(sensor_mon(arr, 3, 1, NULL, &events), STATUS_NULL_PTR);
}

TEST_F(CallbackTest, BothNullReturnsNullPtr) {
    EXPECT_EQ(sensor_mon(NULL, 5, 1, NULL, NULL), STATUS_NULL_PTR);
}

TEST_F(CallbackTest, ZeroLengthReturnsInvalidParam) {
    uint8_t arr[3] = {1, 2, 3};
    EXPECT_EQ(sensor_mon(arr, 0, 1, test_mock_callback, &events), STATUS_INVALID_PARAM);
}

TEST_F(CallbackTest, NoThresholdBreached) {
    uint8_t arr[5] = {1, 2, 3, 4, 5};
    status_t status = sensor_mon(arr, 5, 10, test_mock_callback, &events);
    
    EXPECT_EQ(status, STATUS_SUCCESS);
    EXPECT_TRUE(events.empty());
}

TEST_F(CallbackTest, StrictInequalityBoundary) {
    // Check that elements equal to threshold do NOT trigger callback (val > threshold)
    uint8_t arr[4] = {4, 5, 6, 5};
    status_t status = sensor_mon(arr, 4, 5, test_mock_callback, &events);
    
    EXPECT_EQ(status, STATUS_SUCCESS);
    ASSERT_EQ(events.size(), 1u);
    EXPECT_EQ(events[0].val, 6);
    EXPECT_EQ(events[0].index, 2u);
}

TEST_F(CallbackTest, MultipleBreachesAndContextUpdate) {
    uint8_t arr[6] = {1, 3, 7, 8, 2, 5};
    monitor_context_t ctx = {0, 0};
    
    status_t status = sensor_mon(arr, 6, 3, pr_over_temp, &ctx);
    
    EXPECT_EQ(status, STATUS_SUCCESS);
    EXPECT_EQ(ctx.alarm_count, 3u);
    EXPECT_EQ(ctx.max_val, 8u);
}

TEST_F(CallbackTest, NullUserDataDoesNotCrash) {
    uint8_t arr[3] = {10, 20, 30};
    // Passing NULL for user_data with pr_over_temp should not crash
    status_t status = sensor_mon(arr, 3, 5, pr_over_temp, NULL);
    
    EXPECT_EQ(status, STATUS_SUCCESS);
}

TEST_F(CallbackTest, AllElementsBreached) {
    uint8_t arr[4] = {50, 60, 70, 80};
    status_t status = sensor_mon(arr, 4, 20, test_mock_callback, &events);
    
    EXPECT_EQ(status, STATUS_SUCCESS);
    ASSERT_EQ(events.size(), 4u);
    for (size_t i = 0; i < 4; i++) {
        EXPECT_EQ(events[i].val, arr[i]);
        EXPECT_EQ(events[i].index, i);
    }
}
