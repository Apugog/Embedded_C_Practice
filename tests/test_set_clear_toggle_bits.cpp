#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

extern "C" {
    typedef enum {
        BIT_STATUS_OK,
        BIT_STATUS_NULL_PTR,
        BIT_STATUS_INVALID_BIT
    } bit_status_t;

    bit_status_t bit_set(uint32_t *reg, uint8_t bit);
    bit_status_t bit_clear(uint32_t *reg, uint8_t bit);
    bit_status_t bit_toggle(uint32_t *reg, uint8_t bit);
}

/* --------------------------------------------------------------------------
 * 1. bit_set Tests
 * -------------------------------------------------------------------------- */
TEST(BitManipulationTest, BitSet_SingleBits) {
    uint32_t reg = 0x00000000U;

    // Bit 0 (LSB)
    EXPECT_EQ(bit_set(&reg, 0), BIT_STATUS_OK);
    EXPECT_EQ(reg, 0x00000001U);

    // Bit 15 (Mid)
    EXPECT_EQ(bit_set(&reg, 15), BIT_STATUS_OK);
    EXPECT_EQ(reg, 0x00008001U);

    // Bit 31 (MSB)
    EXPECT_EQ(bit_set(&reg, 31), BIT_STATUS_OK);
    EXPECT_EQ(reg, 0x80008001U);
}

TEST(BitManipulationTest, BitSet_Idempotence) {
    uint32_t reg = 0x00000004U; // Bit 2 is already set

    EXPECT_EQ(bit_set(&reg, 2), BIT_STATUS_OK);
    EXPECT_EQ(reg, 0x00000004U);
}

TEST(BitManipulationTest, BitSet_ErrorHandling) {
    uint32_t reg = 0x12345678U;

    // NULL pointer check
    EXPECT_EQ(bit_set(NULL, 5), BIT_STATUS_NULL_PTR);

    // Boundary check: bit 32 is out of range for 32-bit register
    EXPECT_EQ(bit_set(&reg, 32), BIT_STATUS_INVALID_BIT);
    EXPECT_EQ(reg, 0x12345678U); // Ensure register was not corrupted

    // Boundary check: arbitrary out-of-range bit
    EXPECT_EQ(bit_set(&reg, 255), BIT_STATUS_INVALID_BIT);
    EXPECT_EQ(reg, 0x12345678U);
}

/* --------------------------------------------------------------------------
 * 2. bit_clear Tests
 * -------------------------------------------------------------------------- */
TEST(BitManipulationTest, BitClear_SingleBits) {
    uint32_t reg = 0xFFFFFFFFU;

    // Clear Bit 0 (LSB)
    EXPECT_EQ(bit_clear(&reg, 0), BIT_STATUS_OK);
    EXPECT_EQ(reg, 0xFFFFFFFEU);

    // Clear Bit 16
    EXPECT_EQ(bit_clear(&reg, 16), BIT_STATUS_OK);
    EXPECT_EQ(reg, 0xFFFEFFFEU);

    // Clear Bit 31 (MSB)
    EXPECT_EQ(bit_clear(&reg, 31), BIT_STATUS_OK);
    EXPECT_EQ(reg, 0x7FFEFFFEU);
}

TEST(BitManipulationTest, BitClear_Idempotence) {
    uint32_t reg = 0x00000000U; // All bits cleared

    EXPECT_EQ(bit_clear(&reg, 10), BIT_STATUS_OK);
    EXPECT_EQ(reg, 0x00000000U);
}

TEST(BitManipulationTest, BitClear_ErrorHandling) {
    uint32_t reg = 0xABCDEF01U;

    // NULL pointer check
    EXPECT_EQ(bit_clear(NULL, 1), BIT_STATUS_NULL_PTR);

    // Out-of-range checks
    EXPECT_EQ(bit_clear(&reg, 32), BIT_STATUS_INVALID_BIT);
    EXPECT_EQ(reg, 0xABCDEF01U);

    EXPECT_EQ(bit_clear(&reg, 100), BIT_STATUS_INVALID_BIT);
    EXPECT_EQ(reg, 0xABCDEF01U);
}

/* --------------------------------------------------------------------------
 * 3. bit_toggle Tests
 * -------------------------------------------------------------------------- */
TEST(BitManipulationTest, BitToggle_Operation) {
    uint32_t reg = 0x00000000U;

    // Toggle 0 -> 1
    EXPECT_EQ(bit_toggle(&reg, 0), BIT_STATUS_OK);
    EXPECT_EQ(reg, 0x00000001U);

    // Toggle 1 -> 0
    EXPECT_EQ(bit_toggle(&reg, 0), BIT_STATUS_OK);
    EXPECT_EQ(reg, 0x00000000U);

    // Toggle MSB (bit 31)
    EXPECT_EQ(bit_toggle(&reg, 31), BIT_STATUS_OK);
    EXPECT_EQ(reg, 0x80000000U);

    EXPECT_EQ(bit_toggle(&reg, 31), BIT_STATUS_OK);
    EXPECT_EQ(reg, 0x00000000U);
}

TEST(BitManipulationTest, BitToggle_ErrorHandling) {
    uint32_t reg = 0x55555555U;

    // NULL pointer check
    EXPECT_EQ(bit_toggle(NULL, 0), BIT_STATUS_NULL_PTR);

    // Out-of-range checks
    EXPECT_EQ(bit_toggle(&reg, 32), BIT_STATUS_INVALID_BIT);
    EXPECT_EQ(reg, 0x55555555U);

    EXPECT_EQ(bit_toggle(&reg, 64), BIT_STATUS_INVALID_BIT);
    EXPECT_EQ(reg, 0x55555555U);
}

/* --------------------------------------------------------------------------
 * 4. Composite / Hardware Peripheral Simulation Test
 * -------------------------------------------------------------------------- */
TEST(BitManipulationTest, SimulatedPeripheralControlRegister) {
    // Simulating a Control Register (CR):
    // Bit 0: Peripheral Enable (PE)
    // Bit 1: TX Enable (TE)
    // Bit 2: RX Enable (RE)
    // Bit 31: Global Interrupt Enable (GIE)
    uint32_t cr = 0x00000000U;

    // 1. Enable peripheral and TX
    EXPECT_EQ(bit_set(&cr, 0), BIT_STATUS_OK);
    EXPECT_EQ(bit_set(&cr, 1), BIT_STATUS_OK);
    EXPECT_EQ(cr, 0x00000003U);

    // 2. Toggle RX enable on
    EXPECT_EQ(bit_toggle(&cr, 2), BIT_STATUS_OK);
    EXPECT_EQ(cr, 0x00000007U);

    // 3. Enable interrupts (MSB)
    EXPECT_EQ(bit_set(&cr, 31), BIT_STATUS_OK);
    EXPECT_EQ(cr, 0x80000007U);

    // 4. Disable TX
    EXPECT_EQ(bit_clear(&cr, 1), BIT_STATUS_OK);
    EXPECT_EQ(cr, 0x80000005U);

    // 5. Toggle RX off
    EXPECT_EQ(bit_toggle(&cr, 2), BIT_STATUS_OK);
    EXPECT_EQ(cr, 0x80000001U);
}
