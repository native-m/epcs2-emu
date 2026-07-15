#include "r5900_vcpu.h"

#include "core/intrinsics.h"
#include "core/log.h"
#include "mips_instr.h"
#include "mmu.h"
#include <cassert>
#include <cstdint>

namespace epcs2 {

void R5900VCPU::set_pc(uint32_t addr) {
    pc = addr & 0x1FFFFFFF;
    next_pc = pc + 4;
}

void R5900VCPU::tick() {
    current_pc = pc;
    pc = next_pc;
    next_pc += 4;

    uint32_t instruction = MMU::self.read32(current_pc);
    if (instruction == 0) {
        // Skip NOP
        LOG_VERBOSE("[0x%08x] Executed NOP", current_pc);
        return;
    }

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
            LOG_VERBOSE("[0x%08x] Executed J 0x%x (next PC: 0x%x)", current_pc, target << 2, next_pc);
            break;
        }
        case MIPS_OP_JAL: {
            const uint32_t target = decode_target(instruction);
            gpr[31].u64 = current_pc + 8;
            next_pc = (current_pc & 0xF0000000) | (target << 2);
            LOG_VERBOSE("[0x%08x] Executed JAL 0x%x (next PC: 0x%x)", current_pc, target << 2, next_pc);
            break;
        }
        case MIPS_OP_BEQ: {
            const int32_t imm = (int32_t)decode_imm_i16(instruction) << 2;
            const auto [rs, rt] = decode_rs_rt(instruction);
            if (gpr[rs].u64 == gpr[rt].u64) {
                next_pc = current_pc + imm;
                LOG_VERBOSE("[0x%08x] BEQ branch taken. Next PC -> 0x%x", current_pc, next_pc);
            }
            LOG_VERBOSE("[0x%08x] Executed BEQ $%d, $%d, %d", current_pc, rs, rt, imm);
            break;
        }
        case MIPS_OP_BNE: {
            const int32_t imm = (int32_t)decode_imm_i16(instruction) << 2;
            const auto [rs, rt] = decode_rs_rt(instruction);
            if (gpr[rs].u64 != gpr[rt].u64) {
                next_pc = current_pc + imm;
                LOG_VERBOSE("[0x%08x] BNE branch taken. Next PC -> 0x%x", current_pc, next_pc);
            }
            LOG_VERBOSE("[0x%08x] Executed BNE $%d, $%d, %d", current_pc, rs, rt, imm);
            break;
        }
        case MIPS_OP_BLEZ: {
            const int32_t imm = (int32_t)decode_imm_i16(instruction) << 2;
            const auto [rs, rt] = decode_rs_rt(instruction);
            if (gpr[rs].i64 <= 0) {
                next_pc = current_pc + imm;
                LOG_VERBOSE("[0x%08x] BLEZ branch taken. Next PC -> 0x%x", current_pc, next_pc);
            }
            LOG_VERBOSE("[0x%08x] Executed BLEZ $%d, $%d, %d", current_pc, rs, rt, imm);
            break;
        }
        case MIPS_OP_BGTZ: {
            const int32_t imm = (int32_t)decode_imm_i16(instruction) << 2;
            const auto [rs, rt] = decode_rs_rt(instruction);
            if (gpr[rs].i64 > 0) {
                next_pc = current_pc + imm;
                LOG_VERBOSE("[0x%08x] BGTZ branch taken. Next PC -> 0x%x", current_pc, next_pc);
            }
            LOG_VERBOSE("[0x%08x] Executed BGTZ $%d, $%d, %d", current_pc, rs, rt, imm);
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
                LOG_VERBOSE("[0x%08x] ADDI overflow!", current_pc);
            }
            LOG_VERBOSE("[0x%08x] Executed ADDI $%d, $%d, %d", current_pc, rs, rt, b);
            break;
        }
        case MIPS_OP_ADDIU: {
            const int32_t imm = (int32_t)decode_imm_i16(instruction);
            const auto [rs, rt] = decode_rs_rt(instruction);
            gpr[rt].i32 = gpr[rs].i32 + imm;
            LOG_VERBOSE("[0x%08x] Executed ADDIU $%d, $%d, %d", current_pc, rs, rt, imm);
            break;
        }
        case MIPS_OP_SLTI: {
            const int64_t imm = (int64_t)decode_imm_i16(instruction);
            const auto [rs, rt] = decode_rs_rt(instruction);
            gpr[rt].u64 = gpr[rs].i64 < imm;
            LOG_VERBOSE("[0x%08x] Executed SLTI $%d, $%d, %d", current_pc, rs, rt, (int32_t)imm);
            break;
        }
        case MIPS_OP_SLTIU: {
            const uint64_t imm = (int64_t)decode_imm_i16(instruction);
            const auto [rs, rt] = decode_rs_rt(instruction);
            gpr[rt].u64 = gpr[rs].u64 < imm;
            LOG_VERBOSE("[0x%08x] Executed SLTIU $%d, $%d, %d", current_pc, rs, rt, (uint32_t)imm);
            break;
        }
        case MIPS_OP_ANDI: {
            const uint64_t imm = decode_imm_u16(instruction);
            const auto [rs, rt] = decode_rs_rt(instruction);
            gpr[rt].u64 = gpr[rs].u64 & imm;
            LOG_VERBOSE("[0x%08x] Executed ANDI $%d, $%d, %d", current_pc, rs, rt, (uint32_t)imm);
            break;
        }
        case MIPS_OP_ORI: {
            const uint64_t imm = decode_imm_u16(instruction);
            const auto [rs, rt] = decode_rs_rt(instruction);
            gpr[rt].u64 = gpr[rs].u64 | imm;
            LOG_VERBOSE("[0x%08x] Executed ORI $%d, $%d, %d", current_pc, rs, rt, (uint32_t)imm);
            break;
        }
        case MIPS_OP_XORI: {
            const uint64_t imm = decode_imm_u16(instruction);
            const auto [rs, rt] = decode_rs_rt(instruction);
            gpr[rt].u64 = gpr[rs].u64 ^ imm;
            LOG_VERBOSE("[0x%08x] Executed XORI $%d, $%d, %d", current_pc, rs, rt, (uint32_t)imm);
            break;
        }
        case MIPS_OP_LUI: {
            const uint32_t imm = decode_imm_u16(instruction);
            const auto [rs, rt] = decode_rs_rt(instruction);
            gpr[rt].i32 = (int32_t)(imm << 16);
            LOG_VERBOSE("[0x%08x] Executed LUI $%d, $%d, %d", current_pc, rs, rt, imm);
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
        case MIPS_OP_BEQL: {
            const auto [rs, rt] = decode_rs_rt(instruction);
            const int32_t imm = (int32_t)decode_imm_i16(instruction) << 2;
            if (gpr[rs].u64 == gpr[rt].u64) {
                next_pc = current_pc + imm;
                LOG_VERBOSE("[0x%08x] BEQL branch taken. Next PC -> 0x%x", current_pc, next_pc);
            } else {
                pc = next_pc;
                next_pc = pc + 4;
                LOG_VERBOSE("[0x%08x] BEQL branch not taken.", current_pc);
            }
            LOG_VERBOSE("[0x%08x] Executed BEQL $%d, $%d, %d", current_pc, rs, rt, imm);
            break;
        }
        case MIPS_OP_BNEL: {
            const auto [rs, rt] = decode_rs_rt(instruction);
            const int32_t imm = (int32_t)decode_imm_i16(instruction) << 2;
            if (gpr[rs].u64 != gpr[rt].u64) {
                next_pc = current_pc + imm;
                LOG_VERBOSE("[0x%08x] BNEL branch taken. Next PC -> 0x%x", current_pc, next_pc);
            } else {
                pc = next_pc;
                next_pc = pc + 4;
                LOG_VERBOSE("[0x%08x] BNEL branch not taken.", current_pc);
            }
            LOG_VERBOSE("[0x%08x] Executed BNEL $%d, $%d, %d", current_pc, rs, rt, imm);
            break;
        }
        case MIPS_OP_BLEZL: {
            const auto rs = decode_rs(instruction);
            const int32_t imm = (int32_t)decode_imm_i16(instruction) << 2;
            if (gpr[rs].i64 <= 0) {
                next_pc = current_pc + imm;
                LOG_VERBOSE("[0x%08x] BLEZL branch taken. Next PC -> 0x%x", current_pc, next_pc);
            } else {
                pc = next_pc;
                next_pc = pc + 4;
                LOG_VERBOSE("[0x%08x] BLEZL branch not taken.", current_pc);
            }
            LOG_VERBOSE("[0x%08x] Executed BLEZL $%d, %d", current_pc, rs, imm);
            break;
        }
        case MIPS_OP_BGTZL: {
            const auto rs = decode_rs(instruction);
            const int32_t imm = (int32_t)decode_imm_i16(instruction) << 2;
            if (gpr[rs].i64 > 0) {
                next_pc = current_pc + imm;
                LOG_VERBOSE("[0x%08x] BGTZL branch taken. Next PC -> 0x%x", current_pc, next_pc);
            } else {
                pc = next_pc;
                next_pc = pc + 4;
                LOG_VERBOSE("[0x%08x] BLEZL branch not taken.", current_pc);
            }
            LOG_VERBOSE("[0x%08x] Executed BLEZL $%d, %d", current_pc, rs, imm);
            break;
        }
        case MIPS_OP_DADDI: {
            const auto [rs, rt] = decode_rs_rt(instruction);
            const int64_t a = gpr[rt].i64;
            const int64_t b = (int64_t)decode_imm_i16(instruction);
            int64_t result;
            if (!add_overflow(a, b, result)) {
                gpr[rs].i64 = result;
            } else {
                // TODO: handle overflow
            }
            LOG_VERBOSE("[0x%08x] Executed DADDI $%d, $%d, %d", current_pc, rs, rt, (int32_t)b);
            break;
        }
        case MIPS_OP_DADDIU: {
            const int64_t imm = (int64_t)decode_imm_i16(instruction);
            const auto [rs, rt] = decode_rs_rt(instruction);
            gpr[rt].i64 = gpr[rs].i64 + imm;
            LOG_VERBOSE("[0x%08x] Executed DADDIU $%d, $%d, %d", current_pc, rs, rt, (int32_t)imm);
            break;
        }
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
        case MIPS_OP_LB: {
            const auto [rs, rt] = decode_rs_rt(instruction);
            const int32_t offset = decode_imm_i16(instruction);
            const uint32_t addr = gpr[rs].u32 + offset;
            gpr[rt].i64 = (int64_t)MMU::self.read8(addr);
            LOG_VERBOSE("[0x%08x] Executed LB $%d, %d($%d)", current_pc, rt, offset, rs);
            break;
        }
        case MIPS_OP_LH: {
            const auto [rs, rt] = decode_rs_rt(instruction);
            const int32_t offset = decode_imm_i16(instruction);
            const uint32_t addr = gpr[rs].u32 + offset;
            if (addr & 1) {
                LOG_ERROR("[0x%08x] LH unaligned address 0x%x", current_pc, addr);
                break;
            }
            gpr[rt].i64 = (int64_t)MMU::self.read16(addr);
            LOG_VERBOSE("[0x%08x] Executed LH $%d, %d($%d)", current_pc, rt, offset, rs);
            break;
        }
        case MIPS_OP_LWL:
            break;
        case MIPS_OP_LW: {
            const auto [rs, rt] = decode_rs_rt(instruction);
            const int32_t offset = decode_imm_i16(instruction);
            const uint32_t addr = gpr[rs].u32 + offset;
            if (addr & 3) {
                LOG_ERROR("[0x%08x] LW unaligned address 0x%x", current_pc, addr);
                break;
            }
            gpr[rt].i64 = (int64_t)MMU::self.read32(addr);
            LOG_VERBOSE("[0x%08x] Executed LW $%d, %d($%d)", current_pc, rt, offset, rs);
            break;
        }
        case MIPS_OP_LBU:
            break;
        case MIPS_OP_LHU:
            break;
        case MIPS_OP_LWR: {
            const auto [rs, rt] = decode_rs_rt(instruction);
            const int32_t offset = decode_imm_i16(instruction);
            const uint32_t addr = gpr[rs].u32 + offset;
            gpr[rt].i64 = (int64_t)MMU::self.read32(addr);
            LOG_VERBOSE("[0x%08x] Executed LWR $%d, %d($%d)", current_pc, rt, offset, rs);
            break;
        }
        case MIPS_OP_LWU: {
            const auto [rs, rt] = decode_rs_rt(instruction);
            const int32_t offset = decode_imm_i16(instruction);
            const uint32_t addr = gpr[rs].u32 + offset;
            if (addr & 3) {
                LOG_ERROR("[0x%08x] LWU unaligned address 0x%x", current_pc, addr);
                break;
            }
            gpr[rt].u64 = (uint64_t)MMU::self.read32(addr);
            LOG_VERBOSE("[0x%08x] Executed LWU $%d, %d($%d)", current_pc, rt, offset, rs);
            break;
        }
        case MIPS_OP_SB: {
            const auto [rs, rt] = decode_rs_rt(instruction);
            const int32_t offset = decode_imm_i16(instruction);
            const uint32_t addr = gpr[rs].u32 + offset;
            MMU::self.write8(addr, gpr[rt].u8);
            LOG_VERBOSE("[0x%08x] Executed SB $%d, %d($%d)", current_pc, rt, offset, rs);
            break;
        }
        case MIPS_OP_SH: {
            const auto [rs, rt] = decode_rs_rt(instruction);
            const int32_t offset = decode_imm_i16(instruction);
            const uint32_t addr = gpr[rs].u32 + offset;
            if (addr & 1) {
                LOG_ERROR("[0x%08x] SH unaligned address 0x%x", current_pc, addr);
                break;
            }
            MMU::self.write16(addr, gpr[rt].u16);
            LOG_VERBOSE("[0x%08x] Executed SH $%d, %d($%d)", current_pc, rt, offset, rs);
            break;
        }
        case MIPS_OP_SWL:
            break;
        case MIPS_OP_SW: {
            const auto [rs, rt] = decode_rs_rt(instruction);
            const int32_t offset = decode_imm_i16(instruction);
            const uint32_t addr = gpr[rs].u32 + offset;
            if (addr & 3) {
                LOG_ERROR("[0x%08x] SW unaligned address 0x%x", current_pc, addr);
                break;
            }
            MMU::self.write32(addr, gpr[rt].u32);
            LOG_VERBOSE("[0x%08x] Executed SW $%d, %d($%d)", current_pc, rt, offset, rs);
            break;
        }
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
        default:
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
            assert(false && "Syscall");
            break;
        case MIPS_SPECIAL_BREAK:
            assert(false && "Break");
            break;
        case MIPS_SPECIAL_SYNC:
            break;
        case MIPS_SPECIAL_MFHI: {
            const uint32_t rd = decode_rd(instruction);
            gpr[rd].u64 = hi.u64;
            break;
        }
        case MIPS_SPECIAL_MTHI: {
            const uint32_t rs = decode_rs(instruction);
            hi.u64 = gpr[rs].u64;
            break;
        }
        case MIPS_SPECIAL_MFLO: {
            const uint32_t rd = decode_rd(instruction);
            gpr[rd].u64 = lo.u64;
            break;
        }
        case MIPS_SPECIAL_MTLO: {
            const uint32_t rs = decode_rs(instruction);
            lo.u64 = gpr[rs].u64;
            break;
        }
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
        case MIPS_SPECIAL_MFSA: {
            const uint32_t rd = decode_rd(instruction);
            gpr[rd].u64 = reg_sa;
            break;
        }
        case MIPS_SPECIAL_MTSA: {
            const uint32_t rs = decode_rs(instruction);
            reg_sa = gpr[rs].u64;
            break;
        }
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
        }
        default:
            break;
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
        default:
            break;
    }
}

void R5900VCPU::exec_fpu(uint32_t instruction) {
}

void R5900VCPU::exec_mmi(uint32_t instruction) {
    R5900MmiFunction func = decode_func<R5900MmiFunction>(instruction);
    switch (func) {
        case R5900_MMI_MADD: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            const int64_t a = (int64_t)(lo.u64 | (hi.u64 << 32));
            const int64_t b = (int64_t)gpr[rs].i32;
            const int64_t c = (int64_t)gpr[rt].i32;
            const int64_t result = a + b * c;
            lo.i64 = (int64_t)(result & 0xFFFFFFFF);
            hi.i64 = (int64_t)(result >> 32);
            gpr[rd].i64 = lo.i64;
            break;
        }
        case R5900_MMI_MADDU: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            const uint64_t a = (uint64_t)(lo.u64 | (hi.u64 << 32));
            const uint64_t b = (uint64_t)gpr[rs].u32;
            const uint64_t c = (uint64_t)gpr[rt].u32;
            const uint64_t result = a + b * c;
            lo.i64 = (uint64_t)(result & 0xFFFFFFFF);
            hi.i64 = (uint64_t)(result >> 32);
            gpr[rd].i64 = lo.i64;
            break;
        }
        case R5900_MMI_PLZCW:
            break;
        case R5900_MMI_MMI0:
            break;
        case R5900_MMI_MMI2:
            break;
        case R5900_MMI_MFHI1:
            break;
        case R5900_MMI_MTHI1:
            break;
        case R5900_MMI_MFLO1:
            break;
        case R5900_MMI_MTLO1:
            break;
        case R5900_MMI_MULT1:
            break;
        case R5900_MMI_MULTU1:
            break;
        case R5900_MMI_DIV1:
            break;
        case R5900_MMI_DIVU1:
            break;
        case R5900_MMI_MADD1: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            const uint64_t lo_h = lo.u128.u64_2[1] & 0xFFFFFFFF;
            const uint64_t hi_h = hi.u128.u64_2[1] & 0xFFFFFFFF;
            const int64_t a = (int64_t)(lo_h | (hi_h << 32));
            const int64_t b = (int64_t)gpr[rs].i32;
            const int64_t c = (int64_t)gpr[rt].i32;
            const int64_t result = a + b * c;
            lo.i128.i64_2[1] = (int64_t)(result & 0xFFFFFFFF);
            hi.i128.i64_2[1] = (int64_t)(result >> 32);
            gpr[rd].i64 = lo.i64;
            break;
        }
        case R5900_MMI_MADDU1: {
            const auto [rs, rt, rd] = decode_rs_rt_rd(instruction);
            const uint64_t lo_h = lo.u128.u64_2[1] & 0xFFFFFFFF;
            const uint64_t hi_h = hi.u128.u64_2[1] & 0xFFFFFFFF;
            const uint64_t a = (uint64_t)(lo_h | (hi_h << 32));
            const uint64_t b = (uint64_t)gpr[rs].u32;
            const uint64_t c = (uint64_t)gpr[rt].u32;
            const uint64_t result = a + b * c;
            lo.i64 = (uint64_t)(result & 0xFFFFFFFF);
            hi.i64 = (uint64_t)(result >> 32);
            gpr[rd].i64 = lo.i64;
            break;
        }
        case R5900_MMI_MMI1:
            break;
        case R5900_MMI_MMI3:
            break;
        case R5900_MMI_PMFHL:
            break;
        case R5900_MMI_PMTHL:
            break;
        case R5900_MMI_PSLLH: {
            const auto [rt, rd, sa] = decode_rt_rd_sa(instruction);
            for (int i = 0; i < 8; i++) {
                gpr[rd].u128.u16_8[i] = gpr[rd].u128.u16_8[i] << sa;
            }
            break;
        }
        case R5900_MMI_PSRLH: {
            const auto [rt, rd, sa] = decode_rt_rd_sa(instruction);
            for (int i = 0; i < 8; i++) {
                gpr[rd].u128.u16_8[i] = gpr[rd].u128.u16_8[i] >> sa;
            }
            break;
        }
        case R5900_MMI_PSRAH: {
            const auto [rt, rd, sa] = decode_rt_rd_sa(instruction);
            for (int i = 0; i < 8; i++) {
                gpr[rd].i128.i16_8[i] = gpr[rd].i128.i16_8[i] >> sa;
            }
            break;
        }
        case R5900_MMI_PSLLW: {
            const auto [rt, rd, sa] = decode_rt_rd_sa(instruction);
            for (int i = 0; i < 4; i++) {
                gpr[rd].u128.u32_4[i] = gpr[rd].u128.u32_4[i] << sa;
            }
            break;
        }
        case R5900_MMI_PSRLW: {
            const auto [rt, rd, sa] = decode_rt_rd_sa(instruction);
            for (int i = 0; i < 4; i++) {
                gpr[rd].u128.u32_4[i] = gpr[rd].u128.u32_4[i] >> sa;
            }
            break;
        }
        case R5900_MMI_PSRAW: {
            const auto [rt, rd, sa] = decode_rt_rd_sa(instruction);
            for (int i = 0; i < 4; i++) {
                gpr[rd].i128.i32_4[i] = gpr[rd].i128.i32_4[i] >> sa;
            }
            break;
        }
        default:
            break;
    }
}

} // namespace epcs2