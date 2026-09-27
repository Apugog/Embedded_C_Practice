#include <gtest/gtest.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

extern "C" {
    typedef struct {
        uint8_t  sensor_id;
        uint32_t timestamp;
        uint8_t  flag;
        uint32_t reading;
        uint16_t checksum;
    } sensor_payload_t;

    typedef struct {
        uint32_t timestamp;
        uint32_t reading;
        uint16_t checksum;
        uint8_t  sensor_id;
        uint8_t  flag;
    } sensor_payload2_t;

    #pragma pack(push, 1)
    typedef struct {
        uint8_t  sensor_id;
        uint32_t timestamp;
        uint8_t  flag;
        uint32_t reading;
        uint16_t checksum;
    } sensor_payload_packed_t;
    #pragma pack(pop)

    bool unpack_sensor_payload(const sensor_payload_packed_t *src, sensor_payload2_t *dest);
    bool pack_sensor_payload(const sensor_payload2_t *src, sensor_payload_packed_t *dest);
}

/* --------------------------------------------------------------------------
 * 1. Struct Total Size and Padding Verification
 * -------------------------------------------------------------------------- */
TEST(MemoryPaddingTest, TotalStructSizes) {
    // Unoptimized: 1 + 3(pad) + 4 + 1 + 3(pad) + 4 + 2 + 2(tail pad) = 20 bytes
    EXPECT_EQ(sizeof(sensor_payload_t), 20u);

    // Reordered (descending alignment): 4 + 4 + 2 + 1 + 1 = 12 bytes (0 pad)
    EXPECT_EQ(sizeof(sensor_payload2_t), 12u);

    // Packed: 1 + 4 + 1 + 4 + 2 = 12 bytes (forced 0 pad)
    EXPECT_EQ(sizeof(sensor_payload_packed_t), 12u);
}

/* --------------------------------------------------------------------------
 * 2. Member Offsets: Unoptimized Struct
 * -------------------------------------------------------------------------- */
TEST(MemoryPaddingTest, UnoptimizedMemberOffsets) {
    EXPECT_EQ(offsetof(sensor_payload_t, sensor_id), 0u);
    EXPECT_EQ(offsetof(sensor_payload_t, timestamp), 4u); // 3 bytes padding (1..3)
    EXPECT_EQ(offsetof(sensor_payload_t, flag),      8u);
    EXPECT_EQ(offsetof(sensor_payload_t, reading),   12u); // 3 bytes padding (9..11)
    EXPECT_EQ(offsetof(sensor_payload_t, checksum),  16u); // ends at 18; 2 bytes tail pad (18..19)
}

/* --------------------------------------------------------------------------
 * 3. Member Offsets: Reordered Struct (Zero Internal/Tail Padding)
 * -------------------------------------------------------------------------- */
TEST(MemoryPaddingTest, ReorderedMemberOffsets) {
    EXPECT_EQ(offsetof(sensor_payload2_t, timestamp), 0u);
    EXPECT_EQ(offsetof(sensor_payload2_t, reading),   4u);
    EXPECT_EQ(offsetof(sensor_payload2_t, checksum),  8u);
    EXPECT_EQ(offsetof(sensor_payload2_t, sensor_id), 10u);
    EXPECT_EQ(offsetof(sensor_payload2_t, flag),      11u);
}

/* --------------------------------------------------------------------------
 * 4. Member Offsets: Packed Struct (Byte-Aligned Wire Format)
 * -------------------------------------------------------------------------- */
TEST(MemoryPaddingTest, PackedMemberOffsets) {
    EXPECT_EQ(offsetof(sensor_payload_packed_t, sensor_id), 0u);
    EXPECT_EQ(offsetof(sensor_payload_packed_t, timestamp), 1u); // unaligned 4-byte int!
    EXPECT_EQ(offsetof(sensor_payload_packed_t, flag),      5u);
    EXPECT_EQ(offsetof(sensor_payload_packed_t, reading),   6u); // unaligned 4-byte int!
    EXPECT_EQ(offsetof(sensor_payload_packed_t, checksum),  10u);
}

/* --------------------------------------------------------------------------
 * 5. Array Stride and Tail Padding Proof
 * -------------------------------------------------------------------------- */
TEST(MemoryPaddingTest, ArrayStrideGuaranteesElementAlignment) {
    sensor_payload_t arr[2];

    // Array stride must exactly equal sizeof(struct)
    uintptr_t addr0 = (uintptr_t)&arr[0];
    uintptr_t addr1 = (uintptr_t)&arr[1];
    EXPECT_EQ(addr1 - addr0, sizeof(sensor_payload_t));

    // Tail padding guarantees arr[1].timestamp remains aligned to a 4-byte boundary
    uintptr_t timestamp_addr = (uintptr_t)&arr[1].timestamp;
    EXPECT_EQ(timestamp_addr % 4u, 0u);
}

/* --------------------------------------------------------------------------
 * 6. Safe Pack and Unpack Operations (Hardware HardFault Protection)
 * -------------------------------------------------------------------------- */
TEST(MemoryPaddingTest, PackUnpackNullPointerGuards) {
    sensor_payload2_t native = {1000, 42, 0x1234, 5, 1};
    sensor_payload_packed_t packed;

    EXPECT_FALSE(pack_sensor_payload(NULL, &packed));
    EXPECT_FALSE(pack_sensor_payload(&native, NULL));
    EXPECT_FALSE(unpack_sensor_payload(NULL, &native));
    EXPECT_FALSE(unpack_sensor_payload(&packed, NULL));
}

TEST(MemoryPaddingTest, PackAndUnpackRoundTripIntegrity) {
    sensor_payload2_t tx_data = {
        .timestamp = 0xAABBCCDD,
        .reading = 0x11223344,
        .checksum = 0x5566,
        .sensor_id = 0x77,
        .flag = 0x88
    };

    sensor_payload_packed_t wire_packet;
    EXPECT_TRUE(pack_sensor_payload(&tx_data, &wire_packet));

    // Verify wire byte layout
    EXPECT_EQ(wire_packet.sensor_id, 0x77);
    EXPECT_EQ(wire_packet.flag, 0x88);
    EXPECT_EQ(wire_packet.timestamp, 0xAABBCCDDu);
    EXPECT_EQ(wire_packet.reading, 0x11223344u);
    EXPECT_EQ(wire_packet.checksum, 0x5566);

    // Unpack to destination naturally aligned struct
    sensor_payload2_t rx_data;
    memset(&rx_data, 0, sizeof(rx_data));
    EXPECT_TRUE(unpack_sensor_payload(&wire_packet, &rx_data));

    EXPECT_EQ(rx_data.timestamp, tx_data.timestamp);
    EXPECT_EQ(rx_data.reading, tx_data.reading);
    EXPECT_EQ(rx_data.checksum, tx_data.checksum);
    EXPECT_EQ(rx_data.sensor_id, tx_data.sensor_id);
    EXPECT_EQ(rx_data.flag, tx_data.flag);
}
