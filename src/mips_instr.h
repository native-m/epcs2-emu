#pragma once

#include "core/types.h"
#include <tuple>
#include <utility>

namespace epcs2 {

enum MIPSRegisterID {
    MIPS_REG_ZERO = 0,
    MIPS_REG_AT = 1,
    MIPS_REG_V0 = 2,
    MIPS_REG_V1 = 3,
    MIPS_REG_A0 = 4,
    MIPS_REG_A1 = 5,
    MIPS_REG_A2 = 6,
    MIPS_REG_A3 = 7,
    MIPS_REG_T0 = 8,
    MIPS_REG_T1 = 9,
    MIPS_REG_T2 = 10,
    MIPS_REG_T3 = 11,
    MIPS_REG_T4 = 12,
    MIPS_REG_T5 = 13,
    MIPS_REG_T6 = 14,
    MIPS_REG_T7 = 15,
    MIPS_REG_S0 = 16,
    MIPS_REG_S1 = 17,
    MIPS_REG_S2 = 18,
    MIPS_REG_S3 = 19,
    MIPS_REG_S4 = 20,
    MIPS_REG_S5 = 21,
    MIPS_REG_S6 = 22,
    MIPS_REG_S7 = 23,
    MIPS_REG_T8 = 24,
    MIPS_REG_T9 = 25,
    MIPS_REG_K0 = 26,
    MIPS_REG_K1 = 27,
    MIPS_REG_GP = 28,
    MIPS_REG_SP = 29,
    MIPS_REG_FP = 30,
    MIPS_REG_RA = 31,
};

enum MIPSOpcode {
    MIPS_OP_SPECIAL = 0,    // Special functions
    MIPS_OP_REGIMM = 1,     // Register immediate
    MIPS_OP_J = 2,          // Jump
    MIPS_OP_JAL = 3,        // Jump and link
    MIPS_OP_BEQ = 4,        // Branch equal
    MIPS_OP_BNE = 5,        // Branch not equal
    MIPS_OP_BLEZ = 6,       // Branch less than or equal to zero
    MIPS_OP_BGTZ = 7,       // Branch greater than zero
    MIPS_OP_ADDI = 8,       // Add immediate
    MIPS_OP_ADDIU = 9,      // Add immediate unsigned
    MIPS_OP_SLTI = 10,      // Set less than immediate
    MIPS_OP_SLTIU = 11,     // Set less than immediate unsigned
    MIPS_OP_ANDI = 12,      // Bitwise AND immediate
    MIPS_OP_ORI = 13,       // Bitwise OR immediate
    MIPS_OP_XORI = 14,      // Bitwise XOR immediate
    MIPS_OP_LUI = 15,       // Load upper immediate
    MIPS_OP_COP0 = 16,      // Coprocessor 0
    MIPS_OP_COP1 = 17,      // Coprocessor 1
    MIPS_OP_COP2 = 18,      // Coprocessor 2
    MIPS_OP_COP3 = 19,      // Coprocessor 3
    MIPS_OP_BEQL = 20,      // Branch equal likely
    MIPS_OP_BNEL = 21,      // Branch not equal likely
    MIPS_OP_BLEZL = 22,     // Branch less than or equal to zero likely
    MIPS_OP_BGTZL = 23,     // Branch greater than zero likely
    MIPS_OP_DADDI = 24,     // Add doubleword immediate
    MIPS_OP_DADDIU = 25,    // Add doubleword immediate unsigned
    MIPS_OP_LDL = 26,       // Load doubleword left
    MIPS_OP_LDR = 27,       // Load doubleword right
    MIPS_OP_MMI = 28,       // Multimedia instructions
    MIPS_OP_LQ = 30,        // Load quadword
    MIPS_OP_SQ = 31,        // Store quadword
    MIPS_OP_LB = 32,        // Load byte
    MIPS_OP_LH = 33,        // Load halfword
    MIPS_OP_LWL = 34,       // Load word left
    MIPS_OP_LW = 35,        // Load word
    MIPS_OP_LBU = 36,       // Load byte unsigned
    MIPS_OP_LHU = 37,       // Load halfword unsigned
    MIPS_OP_LWR = 38,       // Load word right
    MIPS_OP_LWU = 39,       // Load word unsigned
    MIPS_OP_SB = 40,        // Store byte
    MIPS_OP_SH = 41,        // Store halfword
    MIPS_OP_SWL = 42,       // Store word left
    MIPS_OP_SW = 43,        // Store word
    MIPS_OP_SDL = 44,       // Store doubleword left
    MIPS_OP_SDR = 45,       // Store doubleword right
    MIPS_OP_SWR = 46,       // Store word right
    MIPS_OP_CACHE = 47,     // Cache operation
    MIPS_OP_LWC0 = 48,      // Load word to coprocessor 0 (unused in PS2)
    MIPS_OP_LWC1 = 49,      // Load word to coprocessor 1 (FPU)
    MIPS_OP_LWC2 = 50,      // Load word to coprocessor 2 (unused in PS2)
    MIPS_OP_LWC3_PREF = 51, // Load word to coprocessor 3 or Prefetch
    MIPS_OP_LQC2 = 54,      // Load quadword to coprocessor 2
    MIPS_OP_LD = 55,        // Load doubleword
    MIPS_OP_SWC0 = 56,      // Store word from coprocessor 0 (unused in PS2)
    MIPS_OP_SWC1 = 57,      // Store word from coprocessor 1 (FPU)
    MIPS_OP_SWC2 = 58,      // Store word from coprocessor 2 (unused in PS2)
    MIPS_OP_SWC3 = 59,      // Store word from coprocessor 3 (unused in PS2)
    MIPS_OP_SQC2 = 62,      // Store quadword from coprocessor 2
    MIPS_OP_SD = 63         // Store doubleword
};

enum MIPSSpecialFunction {
    MIPS_SPECIAL_SLL = 0,      // Shift left logical
    MIPS_SPECIAL_SRL = 2,      // Shift right logical
    MIPS_SPECIAL_SRA = 3,      // Shift right arithmetic
    MIPS_SPECIAL_SLLV = 4,     // Shift left logical variable
    MIPS_SPECIAL_SRLV = 6,     // Shift right logical variable
    MIPS_SPECIAL_SRAV = 7,     // Shift right arithmetic variable
    MIPS_SPECIAL_JR = 8,       // Jump register
    MIPS_SPECIAL_JALR = 9,     // Jump and link register
    MIPS_SPECIAL_MOVZ = 10,    // Move conditional zero
    MIPS_SPECIAL_MOVN = 11,    // Move conditional not zero
    MIPS_SPECIAL_SYSCALL = 12, // System call
    MIPS_SPECIAL_BREAK = 13,   // Break
    MIPS_SPECIAL_SYNC = 15,    // Synchronize shared memory
    MIPS_SPECIAL_MFHI = 16,    // Move from HI register
    MIPS_SPECIAL_MTHI = 17,    // Move to HI register
    MIPS_SPECIAL_MFLO = 18,    // Move from LO register
    MIPS_SPECIAL_MTLO = 19,    // Move to LO register
    MIPS_SPECIAL_DSLLV = 20,   // Doubleword shift left logical variable
    MIPS_SPECIAL_DSRLV = 22,   // Doubleword shift right logical variable
    MIPS_SPECIAL_DSRAV = 23,   // Doubleword shift right arithmetic variable
    MIPS_SPECIAL_MULT = 24,    // Multiply
    MIPS_SPECIAL_MULTU = 25,   // Multiply unsigned
    MIPS_SPECIAL_DIV = 26,     // Divide
    MIPS_SPECIAL_DIVU = 27,    // Divide unsigned
    MIPS_SPECIAL_ADD = 32,     // Add
    MIPS_SPECIAL_ADDU = 33,    // Add unsigned
    MIPS_SPECIAL_SUB = 34,     // Subtract
    MIPS_SPECIAL_SUBU = 35,    // Subtract unsigned
    MIPS_SPECIAL_AND = 36,     // Bitwise AND
    MIPS_SPECIAL_OR = 37,      // Bitwise OR
    MIPS_SPECIAL_XOR = 38,     // Bitwise XOR
    MIPS_SPECIAL_NOR = 39,     // Bitwise NOR
    MIPS_SPECIAL_MFSA = 40,    // Move from shift amount register
    MIPS_SPECIAL_MTSA = 41,    // Move to shift amount register
    MIPS_SPECIAL_SLT = 42,     // Set less than
    MIPS_SPECIAL_SLTU = 43,    // Set less than unsigned
    MIPS_SPECIAL_DADD = 44,    // Doubleword add
    MIPS_SPECIAL_DADDU = 45,   // Doubleword add unsigned
    MIPS_SPECIAL_DSUB = 46,    // Doubleword subtract
    MIPS_SPECIAL_DSUBU = 47,   // Doubleword subtract unsigned
    MIPS_SPECIAL_TGE = 48,     // Trap if greater than or equal to
    MIPS_SPECIAL_TGEU = 49,    // Trap if greater than or equal to unsigned
    MIPS_SPECIAL_TLT = 50,     // Trap if less than
    MIPS_SPECIAL_TLTU = 51,    // Trap if less than unsigned
    MIPS_SPECIAL_TEQ = 52,     // Trap if equal
    MIPS_SPECIAL_TNE = 54,     // Trap if not equal
    MIPS_SPECIAL_DSLL = 56,    // Doubleword shift left logical
    MIPS_SPECIAL_DSRL = 58,    // Doubleword shift right logical
    MIPS_SPECIAL_DSRA = 59,    // Doubleword shift right arithmetic
    MIPS_SPECIAL_DSLL32 = 60,  // Doubleword shift left logical 32-bit
    MIPS_SPECIAL_DSRL32 = 62,  // Doubleword shift right logical 32-bit
    MIPS_SPECIAL_DSRA32 = 63   // Doubleword shift right arithmetic 32-bit
};

enum MIPSRegImmFunction {
    MIPS_REGIMM_BLTZ = 0,     // Branch less than zero
    MIPS_REGIMM_BGEZ = 1,     // Branch greater than or equal to zero
    MIPS_REGIMM_BLTZL = 2,    // Branch less than zero and likely
    MIPS_REGIMM_BGEZL = 3,    // Branch greater than or equal to zero and likely
    MIPS_REGIMM_TGEI = 8,     // Trap if greater than or equal to immediate
    MIPS_REGIMM_TGEIU = 9,    // Trap if greater than or equal to unsigned immediate
    MIPS_REGIMM_TLTI = 10,    // Trap if less than immediate
    MIPS_REGIMM_TLTIU = 11,   // Trap if less than unsigned immediate
    MIPS_REGIMM_TEQI = 12,    // Trap if equal immediate
    MIPS_REGIMM_TNEI = 14,    // Trap if not equal immediate
    MIPS_REGIMM_BLTZAL = 16,  // Branch less than zero and link
    MIPS_REGIMM_BGEZAL = 17,  // Branch greater than or equal to zero and link
    MIPS_REGIMM_BLTZALL = 18, // Branch on less than zero and link likely
    MIPS_REGIMM_BGEZALL = 19, // Branch on greater than or equal to zero and link likely
    MIPS_REGIMM_MTSAB = 24,   // Move byte count to shift amount register
    MIPS_REGIMM_MTSAH = 25    // Move halfword count to shift amount register
};

enum MIPSCopFunction {
    MIPS_COP_MF = 0,  // Move from coprocessor
    MIPS_COP_DMF = 1, // Doubleword move from coprocessor
    MIPS_COP_CF = 2,  // Control from coprocessor
    MIPS_COP_MT = 4,  // Move to coprocessor
    MIPS_COP_DMT = 5, // Doubleword move to coprocessor
    MIPS_COP_CT = 6,  // Control to coprocessor
    MIPS_COP_BC = 8   // Branch to coprocessor
};

enum MIPSCop0Function {
    MIPS_COP0_MFC0 = 0,
    MIPS_COP0_DMFC0 = 1,
    MIPS_COP0_MTC0 = 4,
    MIPS_COP0_DMTC0 = 5,
    MIPS_COP0_BC0 = 8
};

enum MIPSFpuSFunction {
    MIPS_FPU_S_ADD_S = 0,
    MIPS_FPU_S_SUB_S = 1,
    MIPS_FPU_S_MUL_S = 2,
    MIPS_FPU_S_DIV_S = 3,
    MIPS_FPU_S_SQRT_S = 4,
    MIPS_FPU_S_ABS_S = 5,
    MIPS_FPU_S_MOV_S = 6,
    MIPS_FPU_S_NEG_S = 7,
    MIPS_FPU_S_RSQRT_S = 22,
    MIPS_FPU_S_ADDA_S = 24,
    MIPS_FPU_S_SUBA_S = 25,
    MIPS_FPU_S_MULA_S = 26,
    MIPS_FPU_S_MADD_S = 28,
    MIPS_FPU_S_MSUB_S = 29,
    MIPS_FPU_S_MADDA_S = 30,
    MIPS_FPU_S_MSUBA_S = 31,
    MIPS_FPU_S_CVT_W = 36,
    MIPS_FPU_S_MAX_S = 40,
    MIPS_FPU_S_MIN_S = 41,
    MIPS_FPU_S_C_F = 48,
    MIPS_FPU_S_C_EQ = 50,
    MIPS_FPU_S_C_LT = 52,
    MIPS_FPU_S_C_LE = 54,
};

enum MIPSFpuWFunction {
    MIPS_FPU_W_CVT_S = 32,
};

enum R5900MmiFunction {
    R5900_MMI_MADD = 0,
    R5900_MMI_MADDU = 1,
    R5900_MMI_PLZCW = 4,
    R5900_MMI_MMI0 = 8,
    R5900_MMI_MMI2 = 9,
    R5900_MMI_MFHI1 = 16,
    R5900_MMI_MTHI1 = 17,
    R5900_MMI_MFLO1 = 18,
    R5900_MMI_MTLO1 = 19,
    R5900_MMI_MULT1 = 24,
    R5900_MMI_MULTU1 = 25,
    R5900_MMI_DIV1 = 26,
    R5900_MMI_DIVU1 = 27,
    R5900_MMI_MADD1 = 32,
    R5900_MMI_MADDU1 = 33,
    R5900_MMI_MMI1 = 40,
    R5900_MMI_MMI3 = 41,
    R5900_MMI_PMFHL = 48,
    R5900_MMI_PMTHL = 49,
    R5900_MMI_PSLLH = 52,
    R5900_MMI_PSRLH = 54,
    R5900_MMI_PSRAH = 55,
    R5900_MMI_PSLLW = 60,
    R5900_MMI_PSRLW = 62,
    R5900_MMI_PSRAW = 63,
};

enum R5900Mmi0Function {
    R5900_MMI0_PADDW = 0,
    R5900_MMI0_PSUBW = 1,
    R5900_MMI0_PCGTW = 2,
    R5900_MMI0_PMAXW = 3,
    R5900_MMI0_PADDH = 4,
    R5900_MMI0_PSUBH = 5,
    R5900_MMI0_PCGTH = 6,
    R5900_MMI0_PMAXH = 7,
    R5900_MMI0_PADDB = 8,
    R5900_MMI0_PSUBB = 9,
    R5900_MMI0_PCGTB = 10,
    R5900_MMI0_PADDSW = 16,
    R5900_MMI0_PSUBSW = 17,
    R5900_MMI0_PEXTLW = 18,
    R5900_MMI0_PPACW = 19,
    R5900_MMI0_PADDSH = 20,
    R5900_MMI0_PSUBSH = 21,
    R5900_MMI0_PEXTLH = 22,
    R5900_MMI0_PPACH = 23,
    R5900_MMI0_PADDSB = 24,
    R5900_MMI0_PSUBSB = 25,
    R5900_MMI0_PEXTLB = 26,
    R5900_MMI0_PPACB = 27,
    R5900_MMI0_PEXT5 = 30,
    R5900_MMI0_PPAC5 = 31,
};

enum R5900Mmi1Function {
    R5900_MMI1_PABSW = 1,
    R5900_MMI1_PCEQW = 2,
    R5900_MMI1_PMINW = 3,
    R5900_MMI1_PADSBH = 4,
    R5900_MMI1_PABSH = 5,
    R5900_MMI1_PCEQH = 6,
    R5900_MMI1_PMINH = 7,
    R5900_MMI1_PCEQB = 10,
    R5900_MMI1_PADDUW = 16,
    R5900_MMI1_PSUBUW = 17,
    R5900_MMI1_PEXTUW = 18,
    R5900_MMI1_PADDUH = 20,
    R5900_MMI1_PSUBUH = 21,
    R5900_MMI1_PEXTUH = 22,
    R5900_MMI1_PADDUB = 24,
    R5900_MMI1_PSUBUB = 25,
    R5900_MMI1_PEXTUB = 26,
    R5900_MMI1_QFSRV = 27,
};

enum R5900Mmi2Function {
    R5900_MMI2_PMADDW = 0,
    R5900_MMI2_PSLLVW = 2,
    R5900_MMI2_PSRLVW = 3,
    R5900_MMI2_PMSUBW = 4,
    R5900_MMI2_PMFHI = 8,
    R5900_MMI2_PMFLO = 9,
    R5900_MMI2_PINTH = 10,
    R5900_MMI2_PMULTW = 12,
    R5900_MMI2_PDIVW = 13,
    R5900_MMI2_PCPYLD = 14,
    R5900_MMI2_PMADDH = 16,
    R5900_MMI2_PHMADH = 17,
    R5900_MMI2_PAND = 18,
    R5900_MMI2_PXOR = 19,
    R5900_MMI2_PMSUBH = 20,
    R5900_MMI2_PHMSBH = 21,
    R5900_MMI2_PEXEH = 26,
    R5900_MMI2_PREVH = 27,
    R5900_MMI2_PMULTH = 28,
    R5900_MMI2_PDIVBW = 29,
    R5900_MMI2_PEXEW = 30,
    R5900_MMI2_PROT3W = 31,
};

enum R5900Mmi3Function {
    R5900_MMI3_PMADDUW = 0,
    R5900_MMI3_PSRAVW = 3,
    R5900_MMI3_PMTHI = 8,
    R5900_MMI3_PMTLO = 9,
    R5900_MMI3_PINTEH = 10,
    R5900_MMI3_PMULTUW = 12,
    R5900_MMI3_PDIVUW = 13,
    R5900_MMI3_PCPYUD = 14,
    R5900_MMI3_POR = 18,
    R5900_MMI3_PNOR = 19,
    R5900_MMI3_PEXCH = 26,
    R5900_MMI3_PCPYH = 27,
    R5900_MMI3_PEXCW = 30,
};

inline MIPSOpcode decode_opcode(uint32_t instruction) {
    return MIPSOpcode((instruction >> 26) & 0x3F);
}

template <typename T>
inline T decode_func(uint32_t instruction) {
    return T(instruction & 0x3F);
}

inline int16_t decode_imm_i16(uint32_t instruction) {
    return (int16_t)(instruction & 0xFFFF);
}

inline uint16_t decode_imm_u16(uint32_t instruction) {
    return instruction & 0xFFFF;
}

inline uint32_t decode_target(uint32_t instruction) {
    return instruction & 0x3ffffff;
}

inline uint32_t decode_rs(uint32_t instruction) {
    return (instruction >> 21) & 0x1f;
}

inline std::pair<uint32_t, uint32_t> decode_rs_rt(uint32_t instruction) {
    return {
        (instruction >> 21) & 0x1f,
        (instruction >> 16) & 0x1f,
    };
}

inline std::pair<uint32_t, uint32_t> decode_rs_rd(uint32_t instruction) {
    return {
        (instruction >> 21) & 0x1f,
        (instruction >> 11) & 0x1f,
    };
}

inline std::tuple<uint32_t, uint32_t, uint32_t> decode_rs_rt_rd(uint32_t instruction) {
    return {
        (instruction >> 21) & 0x1f,
        (instruction >> 16) & 0x1f,
        (instruction >> 11) & 0x1f,
    };
}

inline std::tuple<uint32_t, uint32_t, uint32_t> decode_rt_rd_sa(uint32_t instruction) {
    return {
        (instruction >> 16) & 0x1f,
        (instruction >> 11) & 0x1f,
        (instruction >> 6) & 0x1f,
    };
}

inline uint32_t encode_rtype(uint32_t opcode, uint32_t rs, uint32_t rt, uint32_t rd, uint32_t sa, uint32_t func) {
    return (opcode << 26) | (rs << 21) | (rt << 16) | (rd << 11) | (sa << 6) | func;
}

inline uint32_t encode_itype(uint32_t opcode, uint32_t rs, uint32_t rt, uint16_t immediate) {
    return (opcode << 26) | (rs << 21) | (rt << 16) | immediate;
}

inline uint32_t encode_jtype(uint32_t opcode, uint32_t target) {
    return (opcode << 26) | (target & 0x3ffffff);
}

} // namespace epcs2