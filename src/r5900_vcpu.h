#pragma once

#include "r5900_regs.h"

namespace epcs2 {

struct R5900VCPU {
    R5900Register gpr[32] {};
    R5900RegisterFPU fpr[32] {};

    R5900Register hi {};
    R5900Register lo {};

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
};

} // namespace epcs2