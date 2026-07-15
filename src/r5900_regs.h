#pragma once

#include "core/types.h"

namespace epcs2 {

union R5900Register {
    i128 i128;
    u128 u128;
    int64_t i64;
    uint64_t u64;
    int32_t i32;
    uint32_t u32;
    int16_t i16;
    uint16_t u16;
    int8_t i8;
    uint8_t u8;
};

union R5900RegisterFPU {
    float f32;
    uint32_t u32;
};

} // namespace epcs2