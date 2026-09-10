#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <string>

#define NAME_SIZE 24

extern "C" {
    typedef enum {
        ON_LEAVE = 0,
        ACTIVE
    } emp_status_t;

    typedef enum {
        STATUS_SUCCESS = 0,
        STATUS_NULL_PTR,
        STATUS_INVALID_PARAM,
        STATUS_BUFFER_OVERFLOW
    } record_status_t;

    typedef struct Emp {  
        uint32_t id;    
        uint32_t salary;
        emp_status_t status;
        char name[NAME_SIZE];
    } emp_t;

    record_status_t emp_init(emp_t *emp, uint32_t id, const char *name, 
                             uint32_t salary, emp_status_t status);
    record_status_t emp_update_salary(emp_t *emp, uint32_t new_salary);
    record_status_t emp_update_status(emp_t *emp, emp_status_t new_status);
    record_status_t emp_print(const emp_t *emp);
}

/* --------------------------------------------------------------------------
 * Core Initialization Tests (emp_init)
 * -------------------------------------------------------------------------- */

TEST(EmpInitTest, HandlesValidInitialization) {
    emp_t emp;
    record_status_t status = emp_init(&emp, 101, "Alice Smith", 75000, ACTIVE);

    EXPECT_EQ(status, STATUS_SUCCESS);
    EXPECT_EQ(emp.id, 101u);
    EXPECT_STREQ(emp.name, "Alice Smith");
    EXPECT_EQ(emp.salary, 75000u);
    EXPECT_EQ(emp.status, ACTIVE);
}

TEST(EmpInitTest, FailsOnNullPointer) {
    emp_t emp;
    EXPECT_EQ(emp_init(NULL, 101, "Alice", 50000, ACTIVE), STATUS_NULL_PTR);
    EXPECT_EQ(emp_init(&emp, 101, NULL, 50000, ACTIVE), STATUS_NULL_PTR);
    EXPECT_EQ(emp_init(NULL, 101, NULL, 50000, ACTIVE), STATUS_NULL_PTR);
}

TEST(EmpInitTest, FailsOnZeroId) {
    emp_t emp;
    EXPECT_EQ(emp_init(&emp, 0, "Alice", 50000, ACTIVE), STATUS_INVALID_PARAM);
}

TEST(EmpInitTest, FailsOnEmptyName) {
    emp_t emp;
    EXPECT_EQ(emp_init(&emp, 101, "", 50000, ACTIVE), STATUS_INVALID_PARAM);
}

TEST(EmpInitTest, FailsOnInvalidStatus) {
    emp_t emp;
    EXPECT_EQ(emp_init(&emp, 101, "Alice", 50000, (emp_status_t)2), STATUS_INVALID_PARAM);
    EXPECT_EQ(emp_init(&emp, 101, "Alice", 50000, (emp_status_t)99), STATUS_INVALID_PARAM);
}

TEST(EmpInitTest, FailsOnNameBufferOverflowExactSize) {
    emp_t emp;
    // Buffer is 24 bytes. 24 characters + '\0' needs 25 bytes.
    const char name_24_chars[] = "123456789012345678901234";
    ASSERT_EQ(strlen(name_24_chars), 24u);

    EXPECT_EQ(emp_init(&emp, 101, name_24_chars, 50000, ACTIVE), STATUS_BUFFER_OVERFLOW);
}

TEST(EmpInitTest, FailsOnNameBufferOverflowLarge) {
    emp_t emp;
    const char name_long[] = "This is a ridiculously long name for an employee struct";
    EXPECT_EQ(emp_init(&emp, 101, name_long, 50000, ACTIVE), STATUS_BUFFER_OVERFLOW);
}

TEST(EmpInitTest, SucceedsOnMaxAllowedNameLength) {
    emp_t emp;
    // Buffer is 24 bytes. 23 characters + '\0' = 24 bytes.
    const char name_23_chars[] = "12345678901234567890123";
    ASSERT_EQ(strlen(name_23_chars), 23u);

    EXPECT_EQ(emp_init(&emp, 101, name_23_chars, 50000, ACTIVE), STATUS_SUCCESS);
    EXPECT_STREQ(emp.name, name_23_chars);
}

TEST(EmpInitTest, ZerosMemoryToPreventLeakage) {
    emp_t emp;
    // Pre-fill memory with garbage pattern
    memset(&emp, 0xAA, sizeof(emp_t));

    record_status_t status = emp_init(&emp, 1, "Bob", 1000, ON_LEAVE);
    EXPECT_EQ(status, STATUS_SUCCESS);
    EXPECT_EQ(emp.id, 1u);
    EXPECT_STREQ(emp.name, "Bob");
    // Trailing bytes of name buffer must be zeroed
    for (size_t i = strlen("Bob"); i < sizeof(emp.name); ++i) {
        EXPECT_EQ(emp.name[i], '\0');
    }
}

/* --------------------------------------------------------------------------
 * Mutator Tests (emp_update_salary, emp_update_status)
 * -------------------------------------------------------------------------- */

TEST(EmpMutatorTest, UpdateSalarySuccess) {
    emp_t emp;
    emp_init(&emp, 101, "Alice", 50000, ACTIVE);

    EXPECT_EQ(emp_update_salary(&emp, 65000), STATUS_SUCCESS);
    EXPECT_EQ(emp.salary, 65000u);
}

TEST(EmpMutatorTest, UpdateSalaryFailsOnNullEmp) {
    EXPECT_EQ(emp_update_salary(NULL, 65000), STATUS_NULL_PTR);
}

TEST(EmpMutatorTest, UpdateSalaryFailsOnZero) {
    emp_t emp;
    emp_init(&emp, 101, "Alice", 50000, ACTIVE);

    EXPECT_EQ(emp_update_salary(&emp, 0), STATUS_INVALID_PARAM);
    EXPECT_EQ(emp.salary, 50000u); // Unchanged
}

TEST(EmpMutatorTest, UpdateStatusSuccess) {
    emp_t emp;
    emp_init(&emp, 101, "Alice", 50000, ACTIVE);

    EXPECT_EQ(emp_update_status(&emp, ON_LEAVE), STATUS_SUCCESS);
    EXPECT_EQ(emp.status, ON_LEAVE);

    EXPECT_EQ(emp_update_status(&emp, ACTIVE), STATUS_SUCCESS);
    EXPECT_EQ(emp.status, ACTIVE);
}

TEST(EmpMutatorTest, UpdateStatusFailsOnNullEmp) {
    EXPECT_EQ(emp_update_status(NULL, ON_LEAVE), STATUS_NULL_PTR);
}

TEST(EmpMutatorTest, UpdateStatusFailsOnInvalidValue) {
    emp_t emp;
    emp_init(&emp, 101, "Alice", 50000, ACTIVE);

    EXPECT_EQ(emp_update_status(&emp, (emp_status_t)5), STATUS_INVALID_PARAM);
    EXPECT_EQ(emp.status, ACTIVE); // Unchanged
}

/* --------------------------------------------------------------------------
 * Inspector Tests (emp_print)
 * -------------------------------------------------------------------------- */

TEST(EmpPrintTest, HandlesNullEmp) {
    EXPECT_EQ(emp_print(NULL), STATUS_NULL_PTR);
}

TEST(EmpPrintTest, PrintsActiveStatusString) {
    emp_t emp;
    emp_init(&emp, 201, "Charlie", 90000, ACTIVE);

    testing::internal::CaptureStdout();
    record_status_t rc = emp_print(&emp);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(rc, STATUS_SUCCESS);
    EXPECT_NE(output.find("Status:ACTIVE"), std::string::npos);
    EXPECT_EQ(output.find("Status:ON_LEAVE"), std::string::npos);
}

TEST(EmpPrintTest, PrintsOnLeaveStatusString) {
    emp_t emp;
    emp_init(&emp, 202, "Dana", 82000, ON_LEAVE);

    testing::internal::CaptureStdout();
    record_status_t rc = emp_print(&emp);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_EQ(rc, STATUS_SUCCESS);
    EXPECT_NE(output.find("Status:ON_LEAVE"), std::string::npos);
    EXPECT_EQ(output.find("Status:ACTIVE"), std::string::npos);
}
