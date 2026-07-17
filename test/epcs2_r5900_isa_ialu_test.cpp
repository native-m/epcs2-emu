#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include "epcs2_r5900_isa_test.h"

using namespace epcs2;

TEST_CASE_METHOD(R5900ISATestFixture, "R5900 IALU ADDI", "[epcs2_r5900_isa_ialu_test]") {
}

TEST_CASE_METHOD(R5900ISATestFixture, "R5900 IALU ADDIU", "[epcs2_r5900_isa_ialu_test]") {
}

TEST_CASE_METHOD(R5900ISATestFixture, "R5900 IALU ADD", "[epcs2_r5900_isa_ialu_test]") {
    push_mov_imm_i32(MIPS_REG_T0, 100);
    push_mov_imm_i32(MIPS_REG_T1, 200);
    push_mov_imm_i32(MIPS_REG_T3, 300);
    push_mov_imm_i32(MIPS_REG_T4, 400);
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T0, MIPS_REG_T1, MIPS_REG_T2, 0, MIPS_SPECIAL_ADD));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T3, MIPS_REG_T4, MIPS_REG_T5, 0, MIPS_SPECIAL_ADD));
    run_program();

    REQUIRE(r5900.gpr[MIPS_REG_T2].i32 == 300);
    REQUIRE(r5900.gpr[MIPS_REG_T5].i32 == 700);
}

TEST_CASE_METHOD(R5900ISATestFixture, "R5900 IALU ADDU", "[epcs2_r5900_isa_ialu_test]") {
    push_mov_imm_i32(MIPS_REG_T0, 100);
    push_mov_imm_i32(MIPS_REG_T1, 200);
    push_mov_imm_i32(MIPS_REG_T3, 300);
    push_mov_imm_i32(MIPS_REG_T4, 400);
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T0, MIPS_REG_T1, MIPS_REG_T2, 0, MIPS_SPECIAL_ADDU));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T3, MIPS_REG_T4, MIPS_REG_T5, 0, MIPS_SPECIAL_ADDU));
    run_program();

    REQUIRE(r5900.gpr[MIPS_REG_T2].i32 == 300);
    REQUIRE(r5900.gpr[MIPS_REG_T5].i32 == 700);
}
