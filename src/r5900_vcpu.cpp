#include "r5900_vcpu.h"

#include "core/intrinsics.h"
#include "mips_instr.h"
#include "sys_memory.h"

namespace epcs2 {

void R5900VCPU::set_pc(uint32_t addr) {
    pc = addr & 0x1FFFFFFF;
    next_pc = pc + 4;
}

void R5900VCPU::tick() {
    current_pc = pc;
    pc = next_pc;
    next_pc += 4;

    uint32_t instruction = SysMemory::self.read32(current_pc);
    MIPSOpcode opcode = decode_opcode(instruction);
    switch (opcode) {
        case MIPS_OP_SPECIAL:
            exec_special(instruction);
            break;
        case MIPS_OP_REGIMM:
            exec_regimm(instruction);
            break;
        case MIPS_OP_J: {
            const uint32_t target = decode_target(instruction);
            next_pc = (current_pc & 0xF0000000) | (target << 2);
            break;
        }
        case MIPS_OP_JAL: {
            const uint32_t target = decode_target(instruction);
            gpr[31].u64 = current_pc + 8;
            next_pc = (current_pc & 0xF0000000) | (target << 2);
            break;
        }
        case MIPS_OP_BEQ: {
            const int32_t imm = (int32_t)decode_imm_i16(instruction) << 2;
            const auto [rs, rt] = decode_rs_rt(instruction);
            if (gpr[rs].u32 == gpr[rt].u32) {
                next_pc = current_pc + imm + 4;
            }
            break;
        }
        case MIPS_OP_BNE: {
            const int32_t imm = (int32_t)decode_imm_i16(instruction) << 2;
            const auto [rs, rt] = decode_rs_rt(instruction);
            if (gpr[rs].u32 != gpr[rt].u32) {
                next_pc = current_pc + imm + 4;
            }
            break;
        }
        case MIPS_OP_BLEZ: {
            const int32_t imm = (int32_t)decode_imm_i16(instruction) << 2;
            const auto [rs, rt] = decode_rs_rt(instruction);
            if (gpr[rs].i32 <= 0) {
                next_pc = current_pc + imm + 4;
            }
            break;
        }
        case MIPS_OP_BGTZ: {
            const int32_t imm = (int32_t)decode_imm_i16(instruction) << 2;
            const auto [rs, rt] = decode_rs_rt(instruction);
            if (gpr[rs].i32 > 0) {
                next_pc = current_pc + imm + 4;
            }
            break;
        }
        case MIPS_OP_ADDI: {
            const auto [rs, rt] = decode_rs_rt(instruction);
            const int32_t a = gpr[rt].i32;
            const int32_t b = (int32_t)decode_imm_i16(instruction);
            int32_t result;
            if (!add_overflow(a, b, result)) {
                gpr[rs].i32 = result;
            } else {
                // TODO: handle overflow
            }
            break;
        }
        case MIPS_OP_ADDIU: {
            const int32_t imm = (int32_t)decode_imm_i16(instruction);
            const auto [rs, rt] = decode_rs_rt(instruction);
            gpr[rt].i32 = gpr[rs].i32 + imm;
            break;
        }
        case MIPS_OP_SLTI: {
            const int64_t imm = (int64_t)decode_imm_i16(instruction);
            const auto [rs, rt] = decode_rs_rt(instruction);
            gpr[rt].u64 = gpr[rs].i64 < imm;
            break;
        }
        case MIPS_OP_SLTIU: {
            const int64_t imm = (int64_t)decode_imm_i16(instruction);
            const auto [rs, rt] = decode_rs_rt(instruction);
            gpr[rt].u64 = gpr[rs].u64 < imm;
            break;
        }
        case MIPS_OP_ANDI: {
            const uint32_t imm = decode_imm_u16(instruction);
            const auto [rs, rt] = decode_rs_rt(instruction);
            gpr[rt].u32 = gpr[rs].u32 & imm;
            break;
        }
        case MIPS_OP_ORI: {
            const uint32_t imm = decode_imm_u16(instruction);
            const auto [rs, rt] = decode_rs_rt(instruction);
            gpr[rt].u32 = gpr[rs].u32 | imm;
            break;
        }
        case MIPS_OP_XORI: {
            const uint32_t imm = decode_imm_u16(instruction);
            const auto [rs, rt] = decode_rs_rt(instruction);
            gpr[rt].u32 = gpr[rs].u32 ^ imm;
            break;
        }
        case MIPS_OP_LUI: {
            const uint32_t imm = decode_imm_u16(instruction);
            const auto [rs, rt] = decode_rs_rt(instruction);
            gpr[rt].i32 = (int32_t)(imm << 16);
            break;
        }
        case MIPS_OP_COP0:
            break;
        case MIPS_OP_COP1:
            exec_fpu(instruction);
            break;
        case MIPS_OP_COP2:
            break;
        case MIPS_OP_COP3:
            break;
        case MIPS_OP_BEQL:
            break;
        case MIPS_OP_BNEL:
            break;
        case MIPS_OP_BLEZL:
            break;
        case MIPS_OP_BGTZL:
            break;
        case MIPS_OP_DADDI:
            break;
        case MIPS_OP_DADDIU:
            break;
        case MIPS_OP_LDL:
            break;
        case MIPS_OP_LDR:
            break;
        case MIPS_OP_MMI:
            break;
        case MIPS_OP_LQ:
            break;
        case MIPS_OP_SQ:
            break;
        case MIPS_OP_LB:
            break;
        case MIPS_OP_LH:
            break;
        case MIPS_OP_LWL:
            break;
        case MIPS_OP_LW:
            break;
        case MIPS_OP_LBU:
            break;
        case MIPS_OP_LHU:
            break;
        case MIPS_OP_LWR:
            break;
        case MIPS_OP_LWU:
            break;
        case MIPS_OP_SB:
            break;
        case MIPS_OP_SH:
            break;
        case MIPS_OP_SWL:
            break;
        case MIPS_OP_SW:
            break;
        case MIPS_OP_SDL:
            break;
        case MIPS_OP_SDR:
            break;
        case MIPS_OP_SWR:
            break;
        case MIPS_OP_CACHE:
            break;
        case MIPS_OP_LWC0:
            break;
        case MIPS_OP_LWC1:
            break;
        case MIPS_OP_LWC2:
            break;
        case MIPS_OP_LWC3_PREF:
            break;
        case MIPS_OP_LQC2:
            break;
        case MIPS_OP_LD:
            break;
        case MIPS_OP_SWC0:
            break;
        case MIPS_OP_SWC1:
            break;
        case MIPS_OP_SWC2:
            break;
        case MIPS_OP_SWC3:
            break;
        case MIPS_OP_SQC2:
            break;
        case MIPS_OP_SD:
            break;
    }
}

void R5900VCPU::exec_special(uint32_t instruction) {
    MIPSSpecialFunction func = decode_func<MIPSSpecialFunction>(instruction);
    switch (func) {
        case MIPS_SPECIAL_SLL: {
            const auto [rt, rd, sa] = decode_rt_rd_sa(instruction);
            gpr[rd].u32 = gpr[rt].u32 << sa;
            break;
        }
        case MIPS_SPECIAL_SRL: {
            const auto [rt, rd, sa] = decode_rt_rd_sa(instruction);
            gpr[rd].u32 = gpr[rt].u32 >> sa;
            break;
        }
        case MIPS_SPECIAL_SRA: {
            const auto [rt, rd, sa] = decode_rt_rd_sa(instruction);
            gpr[rd].i32 = gpr[rt].i32 >> sa;
            break;
        }
        case MIPS_SPECIAL_SLLV: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            gpr[rd].u32 = gpr[rt].u32 << gpr[rs].u32;
            break;
        }
        case MIPS_SPECIAL_SRLV: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            gpr[rd].u32 = gpr[rt].u32 >> gpr[rs].u32;
            break;
        }
        case MIPS_SPECIAL_SRAV: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            gpr[rd].i32 = gpr[rt].i32 >> gpr[rs].u32;
            break;
        }
        case MIPS_SPECIAL_JR: {
            const uint32_t rs = decode_rs(instruction);
            next_pc = gpr[rs].u32;
            break;
        }
        case MIPS_SPECIAL_JALR: {
            const auto [rs, rd] = decode_rs_rd(instruction);
            gpr[rd].u64 = current_pc + 8;
            next_pc = gpr[rs].u32;
            break;
        }
        case MIPS_SPECIAL_MOVZ: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            if (gpr[rs].u32 == 0) {
                gpr[rd].u32 = gpr[rt].u32;
            }
            break;
        }
        case MIPS_SPECIAL_MOVN: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            if (gpr[rs].u32 != 0) {
                gpr[rd].u32 = gpr[rt].u32;
            }
            break;
        }
        case MIPS_SPECIAL_SYSCALL:
            break;
        case MIPS_SPECIAL_BREAK:
            break;
        case MIPS_SPECIAL_SYNC:
            break;
        case MIPS_SPECIAL_MFHI:
            break;
        case MIPS_SPECIAL_MTHI:
            break;
        case MIPS_SPECIAL_MFLO:
            break;
        case MIPS_SPECIAL_MTLO:
            break;
        case MIPS_SPECIAL_DSLLV: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            gpr[rd].u64 = gpr[rt].u64 << gpr[rs].u64;
            break;
        }
        case MIPS_SPECIAL_DSRLV: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            gpr[rd].u64 = gpr[rt].u64 >> gpr[rs].u64;
            break;
        }
        case MIPS_SPECIAL_DSRAV: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            gpr[rd].i64 = gpr[rt].i64 >> gpr[rs].u64;
            break;
        }
        case MIPS_SPECIAL_MULT: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            const int64_t result = (int64_t)gpr[rs].i32 * (int64_t)gpr[rt].i32;
            lo.u64 = (uint64_t)result;
            hi.u64 = (uint64_t)result >> 32;
            break;
        }
        case MIPS_SPECIAL_MULTU: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            const uint64_t result = gpr[rs].u32 * gpr[rt].u32;
            lo.u64 = result;
            hi.u64 = result >> 32;
            break;
        }
        case MIPS_SPECIAL_DIV: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            const int32_t a = gpr[rs].i32;
            const int32_t b = gpr[rt].i32;
            if (b == 0) [[unlikely]] {
                lo.u64 = a >= 0 ? ~0U : 1u;
                hi.u64 = (uint32_t)a;
            } else if (a == INT32_MIN && b == -1) [[unlikely]] {
                // Does not raise exception, instead return these following values
                lo.u64 = 0x80000000;
                hi.u64 = 0;
            } else [[likely]] {
                lo.u64 = (uint32_t)(a / b);
                hi.u64 = (uint32_t)(a % b);
            }
            break;
        }
        case MIPS_SPECIAL_DIVU: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            const uint32_t a = gpr[rs].u32;
            const uint32_t b = gpr[rt].u32;
            if (b == 0) [[unlikely]] {
                lo.u64 = ~0U;
                hi.u64 = a;
            } else [[likely]] {
                lo.u64 = a / b;
                hi.u64 = a % b;
            }
            break;
        }
        case MIPS_SPECIAL_ADD: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            const int32_t a = gpr[rs].i32;
            const int32_t b = gpr[rt].i32;
            int32_t result;
            if (!add_overflow(a, b, result)) {
                gpr[rd].i64 = (int64_t)result;
            } else {
                // TODO: handle overflow
            }
            break;
        }
        case MIPS_SPECIAL_ADDU: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            gpr[rd].i64 = (int64_t)(gpr[rs].i32 + gpr[rt].i32);
            break;
        }
        case MIPS_SPECIAL_SUB: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            const int32_t a = gpr[rs].i32;
            const int32_t b = gpr[rt].i32;
            int32_t result;
            if (!sub_overflow(a, b, result)) {
                gpr[rd].i64 = (int64_t)result;
            } else {
                // TODO: handle overflow
            }
            break;
        }
        case MIPS_SPECIAL_SUBU: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            gpr[rd].i64 = (int64_t)(gpr[rs].i32 - gpr[rt].i32);
            break;
        }
        case MIPS_SPECIAL_AND: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            gpr[rd].u64 = gpr[rs].u64 & gpr[rt].u64;
            break;
        }
        case MIPS_SPECIAL_OR: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            gpr[rd].u64 = gpr[rs].u64 | gpr[rt].u64;
            break;
        }
        case MIPS_SPECIAL_XOR: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            gpr[rd].u64 = gpr[rs].u64 ^ gpr[rt].u64;
            break;
        }
        case MIPS_SPECIAL_NOR: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            gpr[rd].u64 = ~(gpr[rs].u64 | gpr[rt].u64);
            break;
        }
        case MIPS_SPECIAL_MFSA:
            break;
        case MIPS_SPECIAL_MTSA:
            break;
        case MIPS_SPECIAL_SLT: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            gpr[rd].u64 = gpr[rs].i64 < gpr[rt].i64;
            break;
        }
        case MIPS_SPECIAL_SLTU: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            gpr[rd].u64 = gpr[rs].u64 < gpr[rt].u64;
            break;
        }
        case MIPS_SPECIAL_DADD: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            const int64_t a = gpr[rs].i64;
            const int64_t b = gpr[rt].i64;
            int64_t result;
            if (!add_overflow(a, b, result)) {
                gpr[rd].i64 = result;
            } else {
                // TODO: handle overflow
            }
            break;
        }
        case MIPS_SPECIAL_DADDU: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            gpr[rd].i64 = gpr[rs].i64 + gpr[rt].i64;
            break;
        }
        case MIPS_SPECIAL_DSUB: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            const int64_t a = gpr[rs].i64;
            const int64_t b = gpr[rt].i64;
            int64_t result;
            if (!sub_overflow(a, b, result)) {
                gpr[rd].i64 = result;
            } else {
                // TODO: handle overflow
            }
            break;
        }
        case MIPS_SPECIAL_DSUBU: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            gpr[rd].i64 = gpr[rs].i64 - gpr[rt].i64;
            break;
        }
        case MIPS_SPECIAL_TGE:
            break;
        case MIPS_SPECIAL_TGEU:
            break;
        case MIPS_SPECIAL_TLT:
            break;
        case MIPS_SPECIAL_TLTU:
            break;
        case MIPS_SPECIAL_TEQ:
            break;
        case MIPS_SPECIAL_TNE:
            break;
        case MIPS_SPECIAL_DSLL: {
            const auto [rt, rd, sa] = decode_rt_rd_sa(instruction);
            gpr[rd].u64 = gpr[rt].u64 << sa;
            break;
        }
        case MIPS_SPECIAL_DSRL: {
            const auto [rt, rd, sa] = decode_rt_rd_sa(instruction);
            gpr[rd].u64 = gpr[rt].u64 >> sa;
            break;
        }
        case MIPS_SPECIAL_DSRA: {
            const auto [rt, rd, sa] = decode_rt_rd_sa(instruction);
            gpr[rd].i64 = gpr[rt].i64 >> sa;
            break;
        }
        case MIPS_SPECIAL_DSLL32: {
            const auto [rt, rd, sa] = decode_rt_rd_sa(instruction);
            gpr[rd].u64 = gpr[rt].u64 << (sa + 32);
            break;
        }
        case MIPS_SPECIAL_DSRL32: {
            const auto [rt, rd, sa] = decode_rt_rd_sa(instruction);
            gpr[rd].u64 = gpr[rt].u64 >> (sa + 32);
            break;
        }
        case MIPS_SPECIAL_DSRA32: {
            const auto [rt, rd, sa] = decode_rt_rd_sa(instruction);
            gpr[rd].i64 = gpr[rt].i64 >> (sa + 32);
            break;
        } break;
    }
}

void R5900VCPU::exec_regimm(uint32_t instruction) {
    MIPSRegImmFunction func = decode_func<MIPSRegImmFunction>(instruction);
    switch (func) {
        case MIPS_REGIMM_BLTZ:
            break;
        case MIPS_REGIMM_BGEZ:
            break;
        case MIPS_REGIMM_BLTZL:
            break;
        case MIPS_REGIMM_BGEZL:
            break;
        case MIPS_REGIMM_TGEI:
            break;
        case MIPS_REGIMM_TGEIU:
            break;
        case MIPS_REGIMM_TLTI:
            break;
        case MIPS_REGIMM_TLTIU:
            break;
        case MIPS_REGIMM_TEQI:
            break;
        case MIPS_REGIMM_TNEI:
            break;
        case MIPS_REGIMM_BLTZAL:
            break;
        case MIPS_REGIMM_BGEZAL:
            break;
        case MIPS_REGIMM_BLTZALL:
            break;
        case MIPS_REGIMM_BGEZALL:
            break;
        case MIPS_REGIMM_MTSAB:
            break;
        case MIPS_REGIMM_MTSAH:
            break;
    }
}

void R5900VCPU::exec_fpu(uint32_t instruction) {
}

} // namespace epcs2