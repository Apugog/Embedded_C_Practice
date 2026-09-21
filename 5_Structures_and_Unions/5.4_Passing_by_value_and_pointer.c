#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef enum{
    BMS_OK,
    BMS_ERR_NULL_PTR,
    BMS_ERR_OUT_OF_BOUNDS
}bms_status_t;

typedef struct{
    uint16_t voltage_mv;
    int16_t current_ma;
    int8_t temp_c;
    uint8_t state_of_charge;
    uint16_t fault_flags;
}bms_telemetry_t;

int32_t bms_calculate_power_mw(bms_telemetry_t telemetry){
    int32_t milliwatt;
    milliwatt = (int32_t)(((int64_t)telemetry.voltage_mv*telemetry.current_ma)/1000);
    return milliwatt;
}

void bms_clear_faults_by_val(bms_telemetry_t telemetry){
    telemetry.fault_flags = 0;
    (void)telemetry;
}

bms_status_t bms_clear_faults_by_ptr(bms_telemetry_t *telemetry){
    if(telemetry==NULL){
        return BMS_ERR_NULL_PTR;
    }

    telemetry->fault_flags = 0;
    return BMS_OK;
}

bms_status_t bms_calibration_voltage(bms_telemetry_t *telemetry, int16_t offset_mv){
    if(telemetry == NULL){
        return BMS_ERR_NULL_PTR;
    }

    int32_t new_voltage = (int32_t)telemetry->voltage_mv + offset_mv;
    
    if(new_voltage <0 || new_voltage> (int32_t)UINT16_MAX){
        return BMS_ERR_OUT_OF_BOUNDS;
    }

    telemetry->voltage_mv = (uint16_t)new_voltage;
    return BMS_OK;
}

bms_status_t bms_check_safety_limits(const bms_telemetry_t *telemetry, uint16_t min_v_mv, uint16_t max_v_mv, int8_t max_temp_c, bool *out_is_safe){
    if(telemetry==NULL || out_is_safe==NULL){
        return BMS_ERR_NULL_PTR;
    }

    if(min_v_mv>max_v_mv){
        return BMS_ERR_OUT_OF_BOUNDS;
    }

    if(telemetry->voltage_mv >= min_v_mv && telemetry->voltage_mv <= max_v_mv 
        && telemetry->temp_c <= max_temp_c && telemetry->fault_flags ==0){
        *out_is_safe = true;
    }
    else{
        *out_is_safe = false;
    }

    return BMS_OK;
}

#ifndef TESTING
int main(void){
    bms_telemetry_t battery = {
        .voltage_mv = 3700,
        .current_ma = 1500,
        .temp_c = 28,
        .state_of_charge = 65,
        .fault_flags = 0x0001
    };

    printf("--- BMS Telemetry Demo ---\n");
    printf("Initial: Voltage=%u mV, Current=%d mA, Temp=%d C, SoC=%u%%, Faults=0x%04X\n",
           battery.voltage_mv, battery.current_ma, battery.temp_c, battery.state_of_charge, battery.fault_flags);

    // 1. Pass-by-Value: Power calculation
    int32_t power_mw = bms_calculate_power_mw(battery);
    printf("1. Pass-by-Value Power: %ld mW\n", (long)power_mw);

    // 2. Pass-by-Value: Attempt fault clear (proves caller isolation)
    bms_clear_faults_by_val(battery);
    printf("2. After bms_clear_faults_by_val: Faults=0x%04X (unchanged!)\n", battery.fault_flags);

    // 3. Pass-by-Pointer: Clear faults (mutates caller struct)
    bms_status_t status = bms_clear_faults_by_ptr(&battery);
    printf("3. After bms_clear_faults_by_ptr: Status=%d, Faults=0x%04X (cleared!)\n", status, battery.fault_flags);

    // 4. Pass-by-Pointer: Calibrate voltage (mutator with bounds checking)
    status = bms_calibration_voltage(&battery, 50);
    printf("4. After bms_calibration_voltage (+50 mV): Status=%d, New Voltage=%u mV\n", status, battery.voltage_mv);

    // 5. Pass-by-Const-Pointer: Check safety limits (zero-copy inspector)
    bool is_safe = false;
    status = bms_check_safety_limits(&battery, 3000, 4200, 60, &is_safe);
    printf("5. After bms_check_safety_limits: Status=%d, Safe=%s\n", status, is_safe ? "true" : "false");

    return 0;
}
#endif