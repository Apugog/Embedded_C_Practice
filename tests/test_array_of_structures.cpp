#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

extern "C" {
    #define MAX_SENSORS 8

    typedef enum {
        SENSOR_OK,
        SENSOR_DISCONNECTED,
        SENSOR_OUT_OF_BOUNDS
    } sensor_status_t;

    typedef struct {
        uint8_t sensor_id;
        char name[15];
        int32_t raw_val;
        sensor_status_t status;
    } sensor_t;

    typedef struct {
        sensor_t sensors[MAX_SENSORS];
        size_t count;
    } sensor_registry_t;

    typedef enum {
        REG_OK,
        REG_ERR_NULL_PTR,
        REG_ERR_FULL,
        REG_ERR_DUPLICATE_ID,
        REG_ERR_NOT_FOUND,
        REG_ERR_INVALID_PARAM,
        REG_ERR_EMPTY
    } registry_status_t;

    registry_status_t registry_init(sensor_registry_t *reg);
    registry_status_t registry_add(sensor_registry_t *reg, const sensor_t *sensor);
    registry_status_t registry_find_by_id(sensor_registry_t *reg, uint8_t id, sensor_t **out_sensor);
    registry_status_t registry_update_reading(sensor_registry_t *reg, uint8_t id, int32_t raw_val, sensor_status_t status);
    registry_status_t registry_get_max_reading(const sensor_registry_t *reg, int32_t *out_max_val);
    size_t registry_count_by_state(const sensor_registry_t *reg, sensor_status_t status);
}

static sensor_t make_sensor(uint8_t id, const char *name, int32_t raw_val, sensor_status_t status) {
    sensor_t s;
    memset(&s, 0, sizeof(s));
    s.sensor_id = id;
    if (name) {
        strncpy(s.name, name, sizeof(s.name) - 1);
        s.name[sizeof(s.name) - 1] = '\0';
    }
    s.raw_val = raw_val;
    s.status = status;
    return s;
}

/* --------------------------------------------------------------------------
 * 1. Lifecycle: registry_init
 * -------------------------------------------------------------------------- */
TEST(SensorRegistryTest, InitNullPointer) {
    EXPECT_EQ(registry_init(NULL), REG_ERR_NULL_PTR);
}

TEST(SensorRegistryTest, InitSuccess) {
    sensor_registry_t reg;
    memset(&reg, 0xAA, sizeof(reg)); // Fill with garbage
    EXPECT_EQ(registry_init(&reg), REG_OK);
    EXPECT_EQ(reg.count, 0u);
    for (size_t i = 0; i < MAX_SENSORS; i++) {
        EXPECT_EQ(reg.sensors[i].sensor_id, 0);
        EXPECT_EQ(reg.sensors[i].raw_val, 0);
        EXPECT_EQ(reg.sensors[i].status, SENSOR_OK);
        EXPECT_STREQ(reg.sensors[i].name, "");
    }
}

/* --------------------------------------------------------------------------
 * 2. Registration: registry_add
 * -------------------------------------------------------------------------- */
TEST(SensorRegistryTest, AddNullPointers) {
    sensor_registry_t reg;
    sensor_t sensor = make_sensor(1, "Temp", 25, SENSOR_OK);
    registry_init(&reg);

    EXPECT_EQ(registry_add(NULL, &sensor), REG_ERR_NULL_PTR);
    EXPECT_EQ(registry_add(&reg, NULL), REG_ERR_NULL_PTR);
    EXPECT_EQ(registry_add(NULL, NULL), REG_ERR_NULL_PTR);
}

TEST(SensorRegistryTest, AddInvalidSensorIdZero) {
    sensor_registry_t reg;
    sensor_t sensor = make_sensor(0, "Invalid", 10, SENSOR_OK);
    registry_init(&reg);

    EXPECT_EQ(registry_add(&reg, &sensor), REG_ERR_INVALID_PARAM);
    EXPECT_EQ(reg.count, 0u);
}

TEST(SensorRegistryTest, AddSuccessAndCountIncrement) {
    sensor_registry_t reg;
    registry_init(&reg);

    sensor_t s1 = make_sensor(1, "Pressure", 1013, SENSOR_OK);
    EXPECT_EQ(registry_add(&reg, &s1), REG_OK);
    EXPECT_EQ(reg.count, 1u);
    EXPECT_EQ(reg.sensors[0].sensor_id, 1);
    EXPECT_STREQ(reg.sensors[0].name, "Pressure");
    EXPECT_EQ(reg.sensors[0].raw_val, 1013);
    EXPECT_EQ(reg.sensors[0].status, SENSOR_OK);
}

TEST(SensorRegistryTest, AddDuplicateIdRejection) {
    sensor_registry_t reg;
    registry_init(&reg);

    sensor_t s1 = make_sensor(1, "Temp1", 20, SENSOR_OK);
    sensor_t s2 = make_sensor(1, "TempDuplicate", 30, SENSOR_OK);

    EXPECT_EQ(registry_add(&reg, &s1), REG_OK);
    EXPECT_EQ(registry_add(&reg, &s2), REG_ERR_DUPLICATE_ID);
    EXPECT_EQ(reg.count, 1u);
}

TEST(SensorRegistryTest, AddCapacityLimitFull) {
    sensor_registry_t reg;
    registry_init(&reg);

    for (uint8_t i = 1; i <= MAX_SENSORS; i++) {
        sensor_t s = make_sensor(i, "S", 100, SENSOR_OK);
        EXPECT_EQ(registry_add(&reg, &s), REG_OK);
    }
    EXPECT_EQ(reg.count, (size_t)MAX_SENSORS);

    // 9th sensor should be rejected
    sensor_t overflow_sensor = make_sensor(99, "Overflow", 0, SENSOR_OK);
    EXPECT_EQ(registry_add(&reg, &overflow_sensor), REG_ERR_FULL);
    EXPECT_EQ(reg.count, (size_t)MAX_SENSORS);
}

TEST(SensorRegistryTest, AddDefensiveNullTermination) {
    sensor_registry_t reg;
    registry_init(&reg);

    sensor_t s;
    s.sensor_id = 5;
    memset(s.name, 'A', sizeof(s.name)); // 15 non-null bytes
    s.raw_val = 42;
    s.status = SENSOR_OK;

    EXPECT_EQ(registry_add(&reg, &s), REG_OK);
    // Last byte must be safely null-terminated
    EXPECT_EQ(reg.sensors[0].name[sizeof(reg.sensors[0].name) - 1], '\0');
}

/* --------------------------------------------------------------------------
 * 3. Lookup: registry_find_by_id
 * -------------------------------------------------------------------------- */
TEST(SensorRegistryTest, FindNullPointers) {
    sensor_registry_t reg;
    sensor_t *out = NULL;
    registry_init(&reg);

    EXPECT_EQ(registry_find_by_id(NULL, 1, &out), REG_ERR_NULL_PTR);
    EXPECT_EQ(registry_find_by_id(&reg, 1, NULL), REG_ERR_NULL_PTR);
}

TEST(SensorRegistryTest, FindInEmptyRegistry) {
    sensor_registry_t reg;
    sensor_t *out = (sensor_t *)0xDEADBEEF;
    registry_init(&reg);

    EXPECT_EQ(registry_find_by_id(&reg, 1, &out), REG_ERR_EMPTY);
    EXPECT_EQ(out, nullptr);
}

TEST(SensorRegistryTest, FindNonExistentId) {
    sensor_registry_t reg;
    registry_init(&reg);

    sensor_t s = make_sensor(1, "Sensor1", 10, SENSOR_OK);
    registry_add(&reg, &s);

    sensor_t *out = (sensor_t *)0xDEADBEEF;
    EXPECT_EQ(registry_find_by_id(&reg, 99, &out), REG_ERR_NOT_FOUND);
    EXPECT_EQ(out, nullptr);
}

TEST(SensorRegistryTest, FindSuccess) {
    sensor_registry_t reg;
    registry_init(&reg);

    sensor_t s1 = make_sensor(10, "Gyro", 450, SENSOR_OK);
    sensor_t s2 = make_sensor(20, "Accel", -980, SENSOR_OK);
    registry_add(&reg, &s1);
    registry_add(&reg, &s2);

    sensor_t *out = NULL;
    EXPECT_EQ(registry_find_by_id(&reg, 20, &out), REG_OK);
    ASSERT_NE(out, nullptr);
    EXPECT_EQ(out->sensor_id, 20);
    EXPECT_STREQ(out->name, "Accel");
    EXPECT_EQ(out->raw_val, -980);
}

/* --------------------------------------------------------------------------
 * 4. Update: registry_update_reading
 * -------------------------------------------------------------------------- */
TEST(SensorRegistryTest, UpdateNullPointer) {
    EXPECT_EQ(registry_update_reading(NULL, 1, 100, SENSOR_OK), REG_ERR_NULL_PTR);
}

TEST(SensorRegistryTest, UpdateEmptyRegistry) {
    sensor_registry_t reg;
    registry_init(&reg);
    EXPECT_EQ(registry_update_reading(&reg, 1, 100, SENSOR_OK), REG_ERR_EMPTY);
}

TEST(SensorRegistryTest, UpdateInvalidStatus) {
    sensor_registry_t reg;
    registry_init(&reg);
    sensor_t s = make_sensor(1, "Temp", 20, SENSOR_OK);
    registry_add(&reg, &s);

    EXPECT_EQ(registry_update_reading(&reg, 1, 50, (sensor_status_t)99), REG_ERR_INVALID_PARAM);
}

TEST(SensorRegistryTest, UpdateNotFound) {
    sensor_registry_t reg;
    registry_init(&reg);
    sensor_t s = make_sensor(1, "Temp", 20, SENSOR_OK);
    registry_add(&reg, &s);

    EXPECT_EQ(registry_update_reading(&reg, 42, 50, SENSOR_OK), REG_ERR_NOT_FOUND);
}

TEST(SensorRegistryTest, UpdateSuccess) {
    sensor_registry_t reg;
    registry_init(&reg);
    sensor_t s = make_sensor(1, "Temp", 20, SENSOR_OK);
    registry_add(&reg, &s);

    EXPECT_EQ(registry_update_reading(&reg, 1, 85, SENSOR_OUT_OF_BOUNDS), REG_OK);
    EXPECT_EQ(reg.sensors[0].raw_val, 85);
    EXPECT_EQ(reg.sensors[0].status, SENSOR_OUT_OF_BOUNDS);
}

/* --------------------------------------------------------------------------
 * 5. Aggregation: registry_get_max_reading
 * -------------------------------------------------------------------------- */
TEST(SensorRegistryTest, GetMaxNullPointers) {
    sensor_registry_t reg;
    int32_t max_val = 0;
    registry_init(&reg);

    EXPECT_EQ(registry_get_max_reading(NULL, &max_val), REG_ERR_NULL_PTR);
    EXPECT_EQ(registry_get_max_reading(&reg, NULL), REG_ERR_NULL_PTR);
}

TEST(SensorRegistryTest, GetMaxEmptyRegistry) {
    sensor_registry_t reg;
    int32_t max_val = 0;
    registry_init(&reg);

    EXPECT_EQ(registry_get_max_reading(&reg, &max_val), REG_ERR_EMPTY);
}

TEST(SensorRegistryTest, GetMaxAllDisconnected) {
    sensor_registry_t reg;
    registry_init(&reg);

    sensor_t s1 = make_sensor(1, "S1", 100, SENSOR_DISCONNECTED);
    sensor_t s2 = make_sensor(2, "S2", 200, SENSOR_OUT_OF_BOUNDS);
    registry_add(&reg, &s1);
    registry_add(&reg, &s2);

    int32_t max_val = 0;
    EXPECT_EQ(registry_get_max_reading(&reg, &max_val), REG_ERR_NOT_FOUND);
}

TEST(SensorRegistryTest, GetMaxFiltersDisconnectedAndHandlesNegatives) {
    sensor_registry_t reg;
    registry_init(&reg);

    // Sensor 0 is disconnected with a huge reading
    sensor_t s0 = make_sensor(1, "Faulty", 9999, SENSOR_DISCONNECTED);
    // Sensors with negative readings
    sensor_t s1 = make_sensor(2, "Freezer1", -40, SENSOR_OK);
    sensor_t s2 = make_sensor(3, "Freezer2", -10, SENSOR_OK);
    sensor_t s3 = make_sensor(4, "Freezer3", -25, SENSOR_OK);

    registry_add(&reg, &s0);
    registry_add(&reg, &s1);
    registry_add(&reg, &s2);
    registry_add(&reg, &s3);

    int32_t max_val = 0;
    EXPECT_EQ(registry_get_max_reading(&reg, &max_val), REG_OK);
    // Max should be -10 (not 9999 from the disconnected sensor)
    EXPECT_EQ(max_val, -10);
}

/* --------------------------------------------------------------------------
 * 6. Diagnostics: registry_count_by_state
 * -------------------------------------------------------------------------- */
TEST(SensorRegistryTest, CountByStateNullPointer) {
    EXPECT_EQ(registry_count_by_state(NULL, SENSOR_OK), 0u);
}

TEST(SensorRegistryTest, CountByStateInvalidStatus) {
    sensor_registry_t reg;
    registry_init(&reg);
    EXPECT_EQ(registry_count_by_state(&reg, (sensor_status_t)10), 0u);
}

TEST(SensorRegistryTest, CountByStateEmpty) {
    sensor_registry_t reg;
    registry_init(&reg);
    EXPECT_EQ(registry_count_by_state(&reg, SENSOR_OK), 0u);
}

TEST(SensorRegistryTest, CountByStateAccurateCounts) {
    sensor_registry_t reg;
    registry_init(&reg);

    sensor_t s1 = make_sensor(1, "S1", 10, SENSOR_OK);
    sensor_t s2 = make_sensor(2, "S2", 20, SENSOR_OK);
    sensor_t s3 = make_sensor(3, "S3", 30, SENSOR_DISCONNECTED);
    sensor_t s4 = make_sensor(4, "S4", 40, SENSOR_OUT_OF_BOUNDS);
    sensor_t s5 = make_sensor(5, "S5", 50, SENSOR_OK);

    registry_add(&reg, &s1);
    registry_add(&reg, &s2);
    registry_add(&reg, &s3);
    registry_add(&reg, &s4);
    registry_add(&reg, &s5);

    EXPECT_EQ(registry_count_by_state(&reg, SENSOR_OK), 3u);
    EXPECT_EQ(registry_count_by_state(&reg, SENSOR_DISCONNECTED), 1u);
    EXPECT_EQ(registry_count_by_state(&reg, SENSOR_OUT_OF_BOUNDS), 1u);
}
