#include <gtest/gtest.h>
#include <stdint.h>
#include <stdbool.h>

extern "C" {
    typedef enum {
        BMS_OK,
        BMS_ERR_NULL_PTR,
        BMS_ERR_OUT_OF_BOUNDS
    } bms_status_t;

    typedef struct {
        uint16_t voltage_mv;
        int16_t current_ma;
        int8_t temp_c;
        uint8_t state_of_charge;
        uint16_t fault_flags;
    } bms_telemetry_t;

    int32_t bms_calculate_power_mw(bms_telemetry_t telemetry);
    void bms_clear_faults_by_val(bms_telemetry_t telemetry);
    bms_status_t bms_clear_faults_by_ptr(bms_telemetry_t *telemetry);
    bms_status_t bms_calibration_voltage(bms_telemetry_t *telemetry, int16_t offset_mv);
    bms_status_t bms_check_safety_limits(
        const bms_telemetry_t *telemetry,
        uint16_t min_v_mv,
        uint16_t max_v_mv,
        int8_t max_temp_c,
        bool *out_is_safe
    );
}

/* --------------------------------------------------------------------------
 * 1. Struct Memory Layout and Packing Tests
 * -------------------------------------------------------------------------- */
TEST(BmsMemoryLayoutTest, StructSizeAndZeroPadding) {
    // 2 (voltage) + 2 (current) + 1 (temp) + 1 (soc) + 2 (faults) = 8 bytes
    EXPECT_EQ(sizeof(bms_telemetry_t), 8u);
}

/* --------------------------------------------------------------------------
 * 2. Pass-by-Value Tests: Stack Isolation & Immutability
 * -------------------------------------------------------------------------- */
TEST(BmsPassByValueTest, CalculatePowerPositiveCharging) {
    bms_telemetry_t telem = {4200, 1500, 25, 80, 0};
    int32_t power = bms_calculate_power_mw(telem);
    // (4200 * 1500) / 1000 = 6300 mW
    EXPECT_EQ(power, 6300);
}

TEST(BmsPassByValueTest, CalculatePowerNegativeDischarging) {
    bms_telemetry_t telem = {3700, -2000, 30, 50, 0};
    int32_t power = bms_calculate_power_mw(telem);
    // (3700 * -2000) / 1000 = -7400 mW
    EXPECT_EQ(power, -7400);
}

TEST(BmsPassByValueTest, CalculatePowerZeroCurrentOrVoltage) {
    bms_telemetry_t telem1 = {3800, 0, 25, 60, 0};
    EXPECT_EQ(bms_calculate_power_mw(telem1), 0);

    bms_telemetry_t telem2 = {0, 1000, 25, 0, 0};
    EXPECT_EQ(bms_calculate_power_mw(telem2), 0);
}

TEST(BmsPassByValueTest, CalculatePowerLargeValuesSafePromotion) {
    // High voltage pack telemetry: 60000 mV, 30000 mA
    bms_telemetry_t telem = {60000, 30000, 45, 99, 0};
    int32_t power = bms_calculate_power_mw(telem);
    // (60000 * 30000) / 1000 = 1800000 mW
    EXPECT_EQ(power, 1800000);
}

TEST(BmsPassByValueTest, CallerIsolationProvesLocalStackCopy) {
    bms_telemetry_t caller_telem = {3700, 500, 28, 65, 0xABCD};
    
    // Pass by value: function modifies its own stack copy
    bms_clear_faults_by_val(caller_telem);

    // Verify caller's struct was NOT modified
    EXPECT_EQ(caller_telem.fault_flags, 0xABCD);
}

/* --------------------------------------------------------------------------
 * 3. Pass-by-Pointer Mutator Tests
 * -------------------------------------------------------------------------- */
TEST(BmsPassByPointerMutatorTest, ClearFaultsNullPointerRejected) {
    EXPECT_EQ(bms_clear_faults_by_ptr(NULL), BMS_ERR_NULL_PTR);
}

TEST(BmsPassByPointerMutatorTest, ClearFaultsMutatesCallerMemory) {
    bms_telemetry_t caller_telem = {3700, 500, 28, 65, 0xABCD};
    
    // Pass by pointer: mutator modifies caller memory directly
    bms_status_t status = bms_clear_faults_by_ptr(&caller_telem);
    EXPECT_EQ(status, BMS_OK);
    EXPECT_EQ(caller_telem.fault_flags, 0);
}

TEST(BmsPassByPointerMutatorTest, CalibrateVoltageNullPointerRejected) {
    EXPECT_EQ(bms_calibration_voltage(NULL, 50), BMS_ERR_NULL_PTR);
}

TEST(BmsPassByPointerMutatorTest, CalibrateVoltagePositiveOffset) {
    bms_telemetry_t telem = {3700, 0, 25, 50, 0};
    bms_status_t status = bms_calibration_voltage(&telem, 50);
    EXPECT_EQ(status, BMS_OK);
    EXPECT_EQ(telem.voltage_mv, 3750);
}

TEST(BmsPassByPointerMutatorTest, CalibrateVoltageNegativeOffset) {
    bms_telemetry_t telem = {3700, 0, 25, 50, 0};
    bms_status_t status = bms_calibration_voltage(&telem, -100);
    EXPECT_EQ(status, BMS_OK);
    EXPECT_EQ(telem.voltage_mv, 3600);
}

TEST(BmsPassByPointerMutatorTest, CalibrateVoltageUnderflowRejection) {
    bms_telemetry_t telem = {50, 0, 25, 5, 0};
    bms_status_t status = bms_calibration_voltage(&telem, -100);
    // 50 - 100 < 0 -> must be rejected without mutating the struct
    EXPECT_EQ(status, BMS_ERR_OUT_OF_BOUNDS);
    EXPECT_EQ(telem.voltage_mv, 50);
}

TEST(BmsPassByPointerMutatorTest, CalibrateVoltageOverflowRejection) {
    bms_telemetry_t telem = {65500, 0, 25, 95, 0};
    bms_status_t status = bms_calibration_voltage(&telem, 100);
    // 65500 + 100 > 65535 -> must be rejected without mutating the struct
    EXPECT_EQ(status, BMS_ERR_OUT_OF_BOUNDS);
    EXPECT_EQ(telem.voltage_mv, 65500);
}

/* --------------------------------------------------------------------------
 * 4. Pass-by-Const-Pointer Inspector Tests
 * -------------------------------------------------------------------------- */
TEST(BmsPassByConstPointerTest, SafetyLimitsNullPointersRejected) {
    bms_telemetry_t telem = {3700, 0, 25, 50, 0};
    bool is_safe = false;

    EXPECT_EQ(bms_check_safety_limits(NULL, 3000, 4200, 60, &is_safe), BMS_ERR_NULL_PTR);
    EXPECT_EQ(bms_check_safety_limits(&telem, 3000, 4200, 60, NULL), BMS_ERR_NULL_PTR);
}

TEST(BmsPassByConstPointerTest, SafetyLimitsInvalidThresholdsRejected) {
    bms_telemetry_t telem = {3700, 0, 25, 50, 0};
    bool is_safe = false;

    // min_v_mv (4200) > max_v_mv (3000)
    EXPECT_EQ(bms_check_safety_limits(&telem, 4200, 3000, 60, &is_safe), BMS_ERR_OUT_OF_BOUNDS);
}

TEST(BmsPassByConstPointerTest, SafetyLimitsHealthyConditions) {
    bms_telemetry_t telem = {3700, 500, 35, 60, 0};
    bool is_safe = false;

    bms_status_t status = bms_check_safety_limits(&telem, 3000, 4200, 60, &is_safe);
    EXPECT_EQ(status, BMS_OK);
    EXPECT_TRUE(is_safe);
}

TEST(BmsPassByConstPointerTest, SafetyLimitsUnderVoltageDetected) {
    bms_telemetry_t telem = {2900, 500, 25, 10, 0}; // 2900 < 3000
    bool is_safe = true;

    bms_status_t status = bms_check_safety_limits(&telem, 3000, 4200, 60, &is_safe);
    EXPECT_EQ(status, BMS_OK);
    EXPECT_FALSE(is_safe);
}

TEST(BmsPassByConstPointerTest, SafetyLimitsOverVoltageDetected) {
    bms_telemetry_t telem = {4250, 500, 25, 100, 0}; // 4250 > 4200
    bool is_safe = true;

    bms_status_t status = bms_check_safety_limits(&telem, 3000, 4200, 60, &is_safe);
    EXPECT_EQ(status, BMS_OK);
    EXPECT_FALSE(is_safe);
}

TEST(BmsPassByConstPointerTest, SafetyLimitsOverTemperatureDetected) {
    bms_telemetry_t telem = {3700, 500, 65, 50, 0}; // 65 > 60
    bool is_safe = true;

    bms_status_t status = bms_check_safety_limits(&telem, 3000, 4200, 60, &is_safe);
    EXPECT_EQ(status, BMS_OK);
    EXPECT_FALSE(is_safe);
}

TEST(BmsPassByConstPointerTest, SafetyLimitsActiveFaultsDetected) {
    bms_telemetry_t telem = {3700, 500, 25, 50, 0x0001}; // fault flags != 0
    bool is_safe = true;

    bms_status_t status = bms_check_safety_limits(&telem, 3000, 4200, 60, &is_safe);
    EXPECT_EQ(status, BMS_OK);
    EXPECT_FALSE(is_safe);
}

TEST(BmsPassByConstPointerTest, FlashResidentConstDataSafeInspection) {
    // Simulating a ROM/Flash-resident static configuration/telemetry snapshot
    const bms_telemetry_t flash_telemetry = {3800, 0, 20, 70, 0};
    bool is_safe = false;

    // Passing const pointer to const object: zero copy, read-only
    bms_status_t status = bms_check_safety_limits(&flash_telemetry, 3000, 4200, 60, &is_safe);
    EXPECT_EQ(status, BMS_OK);
    EXPECT_TRUE(is_safe);
}
