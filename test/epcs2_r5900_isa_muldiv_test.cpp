#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include "epcs2_r5900_isa_test.h"

using namespace epcs2;

TEST_CASE_METHOD(R5900ISATestFixture, "R5900 MULDIV MULT and MULTU", "[epcs2_r5900_isa_muldiv_test]") {
    push_mov_imm_i32(MIPS_REG_T0, 0x00010000);
    push_mov_imm_i32(MIPS_REG_T1, 0x00010000);
    push_mov_imm_i32(MIPS_REG_T2, 0xFFFFFFFF);
    push_mov_imm_i32(MIPS_REG_T3, 0x00000002);

    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T0, MIPS_REG_T1, MIPS_REG_T4, 0, MIPS_SPECIAL_MULT));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, 0, 0, MIPS_REG_T6, 0, MIPS_SPECIAL_MFLO));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, 0, 0, MIPS_REG_T7, 0, MIPS_SPECIAL_MFHI));

    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T2, MIPS_REG_T3, MIPS_REG_T5, 0, MIPS_SPECIAL_MULTU));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, 0, 0, MIPS_REG_T8, 0, MIPS_SPECIAL_MFLO));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, 0, 0, MIPS_REG_T9, 0, MIPS_SPECIAL_MFHI));
    run_program();

    REQUIRE(r5900.gpr[MIPS_REG_T6].u32 == 0x00000000);
    REQUIRE(r5900.gpr[MIPS_REG_T7].u32 == 0x00000001);
    REQUIRE(r5900.gpr[MIPS_REG_T8].u32 == 0xFFFFFFFE);
    REQUIRE(r5900.gpr[MIPS_REG_T9].u32 == 0x00000001);
}

TEST_CASE_METHOD(R5900ISATestFixture, "R5900 MULDIV DIV and DIVU", "[epcs2_r5900_isa_muldiv_test]") {
    push_mov_imm_i32(MIPS_REG_T0, -10);
    push_mov_imm_i32(MIPS_REG_T1, 3);
    push_mov_imm_i32(MIPS_REG_T2, 10);
    push_mov_imm_i32(MIPS_REG_T3, 3);

    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T0, MIPS_REG_T1, MIPS_REG_T4, 0, MIPS_SPECIAL_DIV));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, 0, 0, MIPS_REG_T5, 0, MIPS_SPECIAL_MFLO));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, 0, 0, MIPS_REG_T6, 0, MIPS_SPECIAL_MFHI));

    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T2, MIPS_REG_T3, MIPS_REG_T7, 0, MIPS_SPECIAL_DIVU));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, 0, 0, MIPS_REG_T8, 0, MIPS_SPECIAL_MFLO));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, 0, 0, MIPS_REG_T9, 0, MIPS_SPECIAL_MFHI));
    run_program();

    REQUIRE(r5900.gpr[MIPS_REG_T5].i32 == -3);
    REQUIRE(r5900.gpr[MIPS_REG_T6].i32 == -1);
    REQUIRE(r5900.gpr[MIPS_REG_T8].u32 == 3);
    REQUIRE(r5900.gpr[MIPS_REG_T9].u32 == 1);
}
