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

union R5900ControlFPU {
    struct {
        uint32_t _pad0 : 3;
        uint32_t underflow_s : 1;
        uint32_t overflow_s : 1;
        uint32_t division_by_zero_s : 1;
        uint32_t invalid_operation_s : 1;
        uint32_t _pad1 : 7;
        uint32_t underflow : 1;
        uint32_t overflow : 1;
        uint32_t division_by_zero : 1;
        uint32_t invalid_operation : 1;
        uint32_t _pad2 : 5;
        uint32_t condition : 1;
    };
    uint32_t u32;
};

} // namespace epcs2