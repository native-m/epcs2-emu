#include "mips_instr.h"
#include "mmu.h"
#include "r5900_vcpu.h"
#include <array>

int main() {
    epcs2::R5900VCPU r5900;
    r5900.set_pc(0x00100000);

    uint32_t program[] = {
        /*0x0 */ epcs2::encode_itype(epcs2::MIPS_OP_ADDI, epcs2::MIPS_REG_A0, epcs2::MIPS_REG_A0, 10),
        /*0x4 */ epcs2::encode_itype(epcs2::MIPS_OP_ADDI, epcs2::MIPS_REG_A1, epcs2::MIPS_REG_A1, 9),
        /*0x8 */ epcs2::encode_itype(epcs2::MIPS_OP_ADDI, epcs2::MIPS_REG_A2, epcs2::MIPS_REG_A2, 10),
        /*0xC */ epcs2::encode_itype(epcs2::MIPS_OP_ADDI, epcs2::MIPS_REG_A3, epcs2::MIPS_REG_A3, 10),
        /*0x10*/ epcs2::encode_itype(epcs2::MIPS_OP_BEQL, epcs2::MIPS_REG_A0, epcs2::MIPS_REG_A1, 3),
        /*0x14*/ epcs2::mips_nop,
        /*0x18*/ epcs2::encode_itype(epcs2::MIPS_OP_ADDI, epcs2::MIPS_REG_A2, epcs2::MIPS_REG_A2, 10),
        /*0x1C*/ epcs2::encode_itype(epcs2::MIPS_OP_ADDI, epcs2::MIPS_REG_A3, epcs2::MIPS_REG_A3, 10),
        /*0x20*/ epcs2::mips_break0,
    };

    epcs2::MMU::self.write_data(0x00100000, program, sizeof(program));

    for (;;) {
        r5900.tick();
    }

    return 0;
}