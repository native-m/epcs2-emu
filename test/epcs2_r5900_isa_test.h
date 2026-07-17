#include "mips_instr.h"
#include "mmu.h"
#include "r5900_vcpu.h"

#include <vector>

struct R5900ISATestFixture {
    epcs2::R5900VCPU r5900;
    std::vector<uint32_t> instructions;
    uint32_t extra_execution_instructions = 0;

    void push_mov_imm_i32(uint32_t dst_reg, int32_t imm_value) {
        uint16_t lo = (uint16_t)imm_value;
        uint16_t hi = (uint16_t)((uint32_t)imm_value >> 16);
        instructions.push_back(epcs2::encode_itype(epcs2::MIPS_OP_LUI, dst_reg, dst_reg, hi));
        instructions.push_back(epcs2::encode_itype(epcs2::MIPS_OP_ORI, dst_reg, dst_reg, lo));
    }
    
    void push_instruction(uint32_t i) { instructions.push_back(i); }

    void push_branch_inst(uint32_t i) {
        instructions.push_back(i);
        extra_execution_instructions++;
    }

    void run_program() {
        uint32_t num_instructions = instructions.size() + extra_execution_instructions;
        epcs2::MMU::self.write(0x00100000, instructions.data(), instructions.size() * sizeof(uint32_t));
        r5900.set_pc(0x00100000);

        for (size_t i = 0; i < num_instructions; i++) {
            r5900.tick();
        }

        epcs2::MMU::self.erase(0x00100000, instructions.size() * sizeof(uint32_t));
    }
};
