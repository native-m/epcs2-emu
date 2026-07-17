#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include "epcs2_r5900_isa_test.h"

using namespace epcs2;

TEST_CASE_METHOD(R5900ISATestFixture, "R5900 IALU ADDI", "[epcs2_r5900_isa_ialu_test]") {
    push_mov_imm_i32(MIPS_REG_T0, 100);
    push_mov_imm_i32(MIPS_REG_T1, 200);
    push_instruction(encode_itype(MIPS_OP_ADDI, MIPS_REG_T0, MIPS_REG_T0, 500));
    push_instruction(encode_itype(MIPS_OP_ADDI, MIPS_REG_T1, MIPS_REG_T1, 500));
    push_instruction(encode_itype(MIPS_OP_ADDI, MIPS_REG_T0, MIPS_REG_T2, 300));
    push_instruction(encode_itype(MIPS_OP_ADDI, MIPS_REG_T1, MIPS_REG_T3, 400));
    run_program();

    REQUIRE(r5900.gpr[MIPS_REG_T0].i32 == 600);
    REQUIRE(r5900.gpr[MIPS_REG_T1].i32 == 700);
    REQUIRE(r5900.gpr[MIPS_REG_T2].i32 == 900);
    REQUIRE(r5900.gpr[MIPS_REG_T3].i32 == 1100);
}

TEST_CASE_METHOD(R5900ISATestFixture, "R5900 IALU ADDIU", "[epcs2_r5900_isa_ialu_test]") {
    push_mov_imm_i32(MIPS_REG_T0, 100);
    push_mov_imm_i32(MIPS_REG_T1, 200);
    push_instruction(encode_itype(MIPS_OP_ADDIU, MIPS_REG_T0, MIPS_REG_T0, 500));
    push_instruction(encode_itype(MIPS_OP_ADDIU, MIPS_REG_T1, MIPS_REG_T1, 500));
    push_instruction(encode_itype(MIPS_OP_ADDI, MIPS_REG_T0, MIPS_REG_T2, 300));
    push_instruction(encode_itype(MIPS_OP_ADDI, MIPS_REG_T1, MIPS_REG_T3, 400));
    run_program();

    REQUIRE(r5900.gpr[MIPS_REG_T0].i32 == 600);
    REQUIRE(r5900.gpr[MIPS_REG_T1].i32 == 700);
    REQUIRE(r5900.gpr[MIPS_REG_T2].i32 == 900);
    REQUIRE(r5900.gpr[MIPS_REG_T3].i32 == 1100);
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

TEST_CASE_METHOD(R5900ISATestFixture, "R5900 IALU SUB", "[epcs2_r5900_isa_ialu_test]") {
    push_mov_imm_i32(MIPS_REG_T0, 100);
    push_mov_imm_i32(MIPS_REG_T1, 200);
    push_mov_imm_i32(MIPS_REG_T3, 500);
    push_mov_imm_i32(MIPS_REG_T4, 400);
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T0, MIPS_REG_T1, MIPS_REG_T2, 0, MIPS_SPECIAL_SUB));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T3, MIPS_REG_T4, MIPS_REG_T5, 0, MIPS_SPECIAL_SUB));
    run_program();

    REQUIRE(r5900.gpr[MIPS_REG_T2].i32 == -100);
    REQUIRE(r5900.gpr[MIPS_REG_T5].i32 == 100);
}

TEST_CASE_METHOD(R5900ISATestFixture, "R5900 IALU SUBU", "[epcs2_r5900_isa_ialu_test]") {
    push_mov_imm_i32(MIPS_REG_T0, 100);
    push_mov_imm_i32(MIPS_REG_T1, 200);
    push_mov_imm_i32(MIPS_REG_T3, 500);
    push_mov_imm_i32(MIPS_REG_T4, 400);
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T0, MIPS_REG_T1, MIPS_REG_T2, 0, MIPS_SPECIAL_SUBU));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T3, MIPS_REG_T4, MIPS_REG_T5, 0, MIPS_SPECIAL_SUBU));
    run_program();

    REQUIRE(r5900.gpr[MIPS_REG_T2].i32 == -100);
    REQUIRE(r5900.gpr[MIPS_REG_T5].i32 == 100);
}