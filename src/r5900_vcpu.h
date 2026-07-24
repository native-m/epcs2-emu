#pragma once

#include "r5900_regs.h"
#include <utility>

namespace epcs2 {

struct R5900VCPU {
    R5900Register gpr[32] {};
    R5900RegisterFPU fpr[32] {};

    R5900Register hi {};
    R5900Register lo {};
    R5900RegisterFPU facc {};

    R5900ControlFPU control_fpu {};

    uint32_t pc {};
    uint32_t current_pc {};
    uint32_t next_pc {};
    uint64_t reg_sa {};

    void set_pc(uint32_t addr);
    void tick();

    void exec_special(uint32_t instruction);
    void exec_regimm(uint32_t instruction);
    void exec_fpu(uint32_t instruction);
    void exec_mmi(uint32_t instruction);

    inline void write_fpr(uint32_t reg, float value) {
        static constexpr uint32_t infinity = 0x7fffffff;
        static constexpr uint32_t max_f32 = 0x7f7fffff;
        fpr[reg].f32 = reg != 0 ? value : 0.0f;

        uint32_t u32_value = fpr[reg].u32;
        // If reaches infinity, set to max float and set overflow flag
        if ((u32_value & 0x7fffffff) == infinity) [[unlikely]] {
            fpr[reg].u32 = (u32_value & 0x80000000) | max_f32;
            control_fpu.overflow = 1;
            control_fpu.overflow_s = 1;
            return;
        } else if (control_fpu.overflow) [[unlikely]] {
            control_fpu.overflow = 0;
        }

        // Check if it's a denormalized number (underflow)
        if (((u32_value & 0x7f800000) == 0) && ((u32_value & 0x007fffff) != 0)) [[unlikely]] {
            fpr[reg].u32 = u32_value & 0x80000000;
            control_fpu.underflow = 1;
            control_fpu.underflow_s = 1;
        } else if (control_fpu.underflow) [[unlikely]] {
            control_fpu.underflow = 0;
        }
    }

    inline void write_facc(float value) {
        static constexpr uint32_t infinity = 0x7fffffff;
        static constexpr uint32_t max_f32 = 0x7f7fffff;
        facc.f32 = value;
        uint32_t u32_value = facc.u32;

        if ((u32_value & 0x7fffffff) == infinity) [[unlikely]] {
            facc.u32 = (u32_value & 0x80000000) | max_f32;
            control_fpu.overflow = 1;
            control_fpu.overflow_s = 1;
            return;
        } else if (control_fpu.overflow) [[unlikely]] {
            control_fpu.overflow = 0;
        }

        if (((u32_value & 0x7f800000) == 0) && ((u32_value & 0x007fffff) != 0)) [[unlikely]] {
            facc.u32 = u32_value & 0x80000000;
            control_fpu.underflow = 1;
            control_fpu.underflow_s = 1;
        } else if (control_fpu.underflow) [[unlikely]] {
            control_fpu.underflow = 0;
        }
    }

    inline void write_fpr2(uint32_t reg, float value) {
        static constexpr uint32_t infinity = 0x7fffffff;
        static constexpr uint32_t max_f32 = 0x7f7fffff;
        fpr[reg].f32 = reg != 0 ? value : 0.0f;
        uint32_t u32_value = fpr[reg].u32;

        if ((u32_value & 0x7fffffff) == infinity) [[unlikely]] {
            fpr[reg].u32 = (u32_value & 0x80000000) | max_f32;
            return;
        }

        if (((u32_value & 0x7f800000) == 0) && ((u32_value & 0x007fffff) != 0)) [[unlikely]] {
            fpr[reg].u32 = u32_value & 0x80000000;
        }
    }
};

} // namespace epcs2