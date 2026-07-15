#pragma once

#include <cstdint>

union i128 {
    struct {
        int64_t lo;
        int64_t hi;
    };
    int64_t i64_2[2];
    int32_t i32_4[4];
    int32_t i16_8[4];
};

union u128 {
    struct {
        uint64_t lo;
        uint64_t hi;
    };
    uint64_t u64_2[2];
    uint32_t u32_4[4];
    uint16_t u16_8[8];
    uint8_t u8_16[16];
};
