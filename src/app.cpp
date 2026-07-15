#include "mips_instr.h"
#include "r5900_vcpu.h"
#include "sys_memory.h"
#include <array>

int main() {
    epcs2::R5900VCPU r5900;
    r5900.set_pc(0x00100000);

    uint32_t program[] = {
        epcs2::encode_itype(epcs2::MIPS_OP_ADDI, epcs2::MIPS_REG_A0, epcs2::MIPS_REG_A0, 10),
        epcs2::encode_itype(epcs2::MIPS_OP_ADDI, epcs2::MIPS_REG_A1, epcs2::MIPS_REG_A1, 10),
        epcs2::encode_itype(epcs2::MIPS_OP_ADDI, epcs2::MIPS_REG_A2, epcs2::MIPS_REG_A2, 10),
        epcs2::encode_itype(epcs2::MIPS_OP_ADDI, epcs2::MIPS_REG_A3, epcs2::MIPS_REG_A3, 10),
    };

    epcs2::SysMemory::self.write_data(0x00100000, program, sizeof(program));

    for (;;) {
        r5900.tick();
    }

    return 0;
}