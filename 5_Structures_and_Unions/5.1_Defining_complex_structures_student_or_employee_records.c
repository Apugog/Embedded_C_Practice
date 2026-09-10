#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define NAME_SIZE 24

typedef enum{
    ON_LEAVE = 0,
    ACTIVE
} emp_status_t;

typedef enum{
    STATUS_SUCCESS = 0,
    STATUS_NULL_PTR,
    STATUS_INVALID_PARAM,
    STATUS_BUFFER_OVERFLOW
} record_status_t;

typedef struct Emp{  
    uint32_t id;    
    uint32_t salary;
    emp_status_t status;
    char name[NAME_SIZE];
} emp_t;

/* --------------------------------------------------------------------------
    * Core Life-Cycle & Initialization
    * -------------------------------------------------------------------------- */

/**
 * @brief  Initializes an employee record in caller-allocated memory.
 * 
 * Invariants & Defensive Checks:
 *  - emp == NULL or name == NULL -> STATUS_NULL_PTR
 *  - id == 0 -> STATUS_INVALID_PARAM (0 is reserved as invalid sentinel)
 *  - status not in {ON_LEAVE, ACTIVE} -> STATUS_INVALID_PARAM
 *  - name length >= sizeof(emp->name) -> STATUS_BUFFER_OVERFLOW
 *  - name is empty "" -> STATUS_INVALID_PARAM
 * 
 * Implementation Details:
 *  - Zero out the struct or unused bytes first (prevents stack leakage).
 *  - Safely copy the string ensuring guaranteed null-termination.
 */
record_status_t emp_init(emp_t *emp, uint32_t id, const char *name, 
    uint32_t salary, emp_status_t status){
        if (emp==NULL || name==NULL){
            return STATUS_NULL_PTR;
        }

        if(id == 0){
            return STATUS_INVALID_PARAM;
        }

        if(status>ACTIVE){
            return STATUS_INVALID_PARAM;
        }

        if(strlen(name) >= sizeof(emp->name)){
            return STATUS_BUFFER_OVERFLOW;
        }

        if(name[0]=='\0'){
            return STATUS_INVALID_PARAM;
        }

        memset(emp, 0, sizeof(emp_t));

        emp->id = id;
        strcpy(emp->name, name);
        emp->salary = salary;
        emp->status = status;
    
        return STATUS_SUCCESS;
}


/* --------------------------------------------------------------------------
* Mutators / Field Updates
* -------------------------------------------------------------------------- */

/**
 * @brief  Updates the salary of an employee record.
 * 
 * Invariants & Defensive Checks:
 *  - emp == NULL -> STATUS_NULL_PTR
 *  - new_salary == 0 -> STATUS_INVALID_PARAM
 */
record_status_t emp_update_salary(emp_t *emp, uint32_t new_salary){
    if(emp==NULL){
        return STATUS_NULL_PTR;
    }

    if(new_salary == 0){
        return STATUS_INVALID_PARAM;
    }

    emp->salary = new_salary;
    return STATUS_SUCCESS;
}

/**
 * @brief  Updates the employment status (ACTIVE / ON_LEAVE).
 * 
 * Invariants & Defensive Checks:
 *  - emp == NULL -> STATUS_NULL_PTR
 *  - new_status is not a valid enum value -> STATUS_INVALID_PARAM
 */
record_status_t emp_update_status(emp_t *emp, emp_status_t new_status){
    if(emp == NULL){
        return STATUS_NULL_PTR;
    }

    if(new_status>ACTIVE){
        return STATUS_INVALID_PARAM;
    }

    emp->status = new_status;
    return STATUS_SUCCESS;
}


/* --------------------------------------------------------------------------
* Inspectors & Formatting
* -------------------------------------------------------------------------- */

/**
 * @brief  Prints the employee record in a formatted, human-readable layout.
 * 
 * Invariants & Defensive Checks:
 *  - emp == NULL -> STATUS_NULL_PTR
 *  - Must print status as a readable string ("ACTIVE" or "ON_LEAVE"), 
 *    not just a raw integer.
 *  - Note const-correctness: function must not modify the record.
 */
record_status_t emp_print(const emp_t *emp){
    if(emp == NULL){
        return STATUS_NULL_PTR;
    }

    const char *status_str = (emp->status == ACTIVE) ? "ACTIVE" : "ON_LEAVE";
    printf(" Employee Info\n =============\n Id:%u\n Name:%s\n Salary:%u\n Status:%s\n\n",
           emp->id, emp->name, emp->salary, status_str);

    return STATUS_SUCCESS;
}

#ifndef TESTING
int main(void) {
    emp_t emp;
    if (emp_init(&emp, 1, "PRAMIT", 50000, ACTIVE) == STATUS_SUCCESS) {
        emp_print(&emp);
        emp_update_salary(&emp, 60000);
        emp_update_status(&emp, ON_LEAVE);
        emp_print(&emp);
    }
    return 0;
}
#endif