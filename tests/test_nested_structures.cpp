#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

extern "C" {
    typedef struct {
        int16_t offset;
        int16_t scale;
        uint8_t is_valid;
    } calib_t;

    typedef struct {
        uint32_t id;
        calib_t calib;
        int16_t last_raw;
        uint8_t is_enabled;
    } sensor_t;

    typedef enum {
        STATUS_OK,
        STATUS_NULL_PTR,
        STATUS_INVALID_PARAM,
        STATUS_UNINITIALIZED
    } sensor_status_t;

    sensor_status_t calib_apply(const calib_t *cal, int16_t raw, int32_t *out);
    sensor_status_t sensor_init(sensor_t *dev, uint32_t id, const calib_t *cal);
    sensor_status_t sensor_read(sensor_t *dev, int16_t raw_val, int32_t *calibrated_out);
}

/* --------------------------------------------------------------------------
 * 1. Memory Layout and Alignment Propagation Tests
 * -------------------------------------------------------------------------- */
TEST(NestedStructuresTest, StructSizeAndAlignment) {
    // calib_t: offset(2) + scale(2) + is_valid(1) + tail pad(1) = 6 bytes
    EXPECT_EQ(sizeof(calib_t), 6u);

    // sensor_t: id(4) + calib(6) + last_raw(2) + is_enabled(1) + tail pad(3) = 16 bytes
    EXPECT_EQ(sizeof(sensor_t), 16u);

    // Offsets verification (descending alignment order, no internal holes)
    EXPECT_EQ(offsetof(sensor_t, id), 0u);
    EXPECT_EQ(offsetof(sensor_t, calib), 4u);
    EXPECT_EQ(offsetof(sensor_t, last_raw), 10u);
    EXPECT_EQ(offsetof(sensor_t, is_enabled), 12u);

    // Nested member offset propagation
    EXPECT_EQ(offsetof(sensor_t, calib) + offsetof(calib_t, offset), 4u);
    EXPECT_EQ(offsetof(sensor_t, calib) + offsetof(calib_t, scale), 6u);
    EXPECT_EQ(offsetof(sensor_t, calib) + offsetof(calib_t, is_valid), 8u);
}

/* --------------------------------------------------------------------------
 * 2. Sub-Object Function: calib_apply() Defensive Checks
 * -------------------------------------------------------------------------- */
TEST(NestedStructuresTest, CalibApplyDefensiveChecks) {
    calib_t cal = { .offset = 10, .scale = 2, .is_valid = 1 };
    int32_t out = 0;

    // NULL pointers
    EXPECT_EQ(calib_apply(NULL, 100, &out), STATUS_NULL_PTR);
    EXPECT_EQ(calib_apply(&cal, 100, NULL), STATUS_NULL_PTR);

    // Uninitialized calibration
    cal.is_valid = 0;
    EXPECT_EQ(calib_apply(&cal, 100, &out), STATUS_UNINITIALIZED);
}

/* --------------------------------------------------------------------------
 * 3. Sub-Object Function: calib_apply() Math & Overflow Safety
 * -------------------------------------------------------------------------- */
TEST(NestedStructuresTest, CalibApplyCalculations) {
    calib_t cal = { .offset = -50, .scale = 3, .is_valid = 1 };
    int32_t out = 0;

    // Normal calculation: (200 * 3) - 50 = 550
    EXPECT_EQ(calib_apply(&cal, 200, &out), STATUS_OK);
    EXPECT_EQ(out, 550);

    // Negative raw reading: (-100 * 3) - 50 = -350
    EXPECT_EQ(calib_apply(&cal, -100, &out), STATUS_OK);
    EXPECT_EQ(out, -350);

    // Max 16-bit boundary math without overflow in 32-bit: (32767 * 32767) + 32767
    calib_t extreme_cal = { .offset = 32767, .scale = 32767, .is_valid = 1 };
    EXPECT_EQ(calib_apply(&extreme_cal, 32767, &out), STATUS_OK);
    EXPECT_EQ(out, (32767L * 32767L) + 32767L);
}

/* --------------------------------------------------------------------------
 * 4. Composite Initialization: sensor_init()
 * -------------------------------------------------------------------------- */
TEST(NestedStructuresTest, SensorInitDefensiveAndCascading) {
    sensor_t dev;

    // NULL pointer
    EXPECT_EQ(sensor_init(NULL, 0x100, NULL), STATUS_NULL_PTR);

    // Invalid ID (0 is reserved)
    EXPECT_EQ(sensor_init(&dev, 0, NULL), STATUS_INVALID_PARAM);

    // Initialize with cal == NULL (uncalibrated device)
    EXPECT_EQ(sensor_init(&dev, 0x101, NULL), STATUS_OK);
    EXPECT_EQ(dev.id, 0x101u);
    EXPECT_EQ(dev.is_enabled, 1u);
    EXPECT_EQ(dev.last_raw, 0);
    EXPECT_EQ(dev.calib.is_valid, 0u);

    // Initialize with valid calibration profile
    calib_t cal = { .offset = 15, .scale = 4, .is_valid = 1 };
    EXPECT_EQ(sensor_init(&dev, 0x202, &cal), STATUS_OK);
    EXPECT_EQ(dev.id, 0x202u);
    EXPECT_EQ(dev.calib.offset, 15);
    EXPECT_EQ(dev.calib.scale, 4);
    EXPECT_EQ(dev.calib.is_valid, 1u);
}

/* --------------------------------------------------------------------------
 * 5. Nested Sub-Object Passing: sensor_read()
 * -------------------------------------------------------------------------- */
TEST(NestedStructuresTest, SensorReadWorkflow) {
    sensor_t dev;
    calib_t cal = { .offset = 20, .scale = 5, .is_valid = 1 };
    int32_t calibrated_val = 0;

    ASSERT_EQ(sensor_init(&dev, 0x303, &cal), STATUS_OK);

    // Defensive NULL checks
    EXPECT_EQ(sensor_read(NULL, 10, &calibrated_val), STATUS_NULL_PTR);
    EXPECT_EQ(sensor_read(&dev, 10, NULL), STATUS_NULL_PTR);

    // Disabled device
    dev.is_enabled = 0;
    EXPECT_EQ(sensor_read(&dev, 10, &calibrated_val), STATUS_INVALID_PARAM);
    dev.is_enabled = 1;

    // Successful read: raw = 40 -> (40 * 5) + 20 = 220
    EXPECT_EQ(sensor_read(&dev, 40, &calibrated_val), STATUS_OK);
    EXPECT_EQ(calibrated_val, 220);
    EXPECT_EQ(dev.last_raw, 40);

    // Sub-object delegation when calibration is uninitialized
    dev.calib.is_valid = 0;
    EXPECT_EQ(sensor_read(&dev, 50, &calibrated_val), STATUS_UNINITIALIZED);
    // Notice: last_raw is still updated to record the telemetry event
    EXPECT_EQ(dev.last_raw, 50);
}
