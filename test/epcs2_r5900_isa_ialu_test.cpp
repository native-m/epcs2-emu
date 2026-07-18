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

TEST_CASE_METHOD(R5900ISATestFixture, "R5900 IALU immediate logic", "[epcs2_r5900_isa_ialu_test]") {
    push_mov_imm_i32(MIPS_REG_T0, 0x12345678);
    push_instruction(encode_itype(MIPS_OP_ANDI, MIPS_REG_T0, MIPS_REG_T1, 0x00F0));
    push_instruction(encode_itype(MIPS_OP_ORI, MIPS_REG_T0, MIPS_REG_T2, 0x00F0));
    push_instruction(encode_itype(MIPS_OP_XORI, MIPS_REG_T0, MIPS_REG_T3, 0x00F0));
    run_program();

    REQUIRE(r5900.gpr[MIPS_REG_T1].i32 == 0x70);
    REQUIRE(r5900.gpr[MIPS_REG_T2].i32 == 0x123456F8);
    REQUIRE(r5900.gpr[MIPS_REG_T3].i32 == 0x12345688);
}

TEST_CASE_METHOD(R5900ISATestFixture, "R5900 IALU compare immediates", "[epcs2_r5900_isa_ialu_test]") {
    push_mov_imm_i32(MIPS_REG_T0, 1);
    push_mov_imm_i32(MIPS_REG_T1, -1);
    push_instruction(encode_itype(MIPS_OP_SLTI, MIPS_REG_T0, MIPS_REG_T2, 2));
    push_instruction(encode_itype(MIPS_OP_SLTI, MIPS_REG_T1, MIPS_REG_T3, 0));
    push_instruction(encode_itype(MIPS_OP_SLTIU, MIPS_REG_T0, MIPS_REG_T4, 2));
    push_instruction(encode_itype(MIPS_OP_SLTIU, MIPS_REG_T1, MIPS_REG_T5, 0));
    run_program();

    REQUIRE(r5900.gpr[MIPS_REG_T2].i32 == 1);
    REQUIRE(r5900.gpr[MIPS_REG_T3].i32 == 1);
    REQUIRE(r5900.gpr[MIPS_REG_T4].i32 == 1);
    REQUIRE(r5900.gpr[MIPS_REG_T5].i32 == 0);
}

TEST_CASE_METHOD(R5900ISATestFixture, "R5900 IALU bitwise register ops", "[epcs2_r5900_isa_ialu_test]") {
    push_mov_imm_i32(MIPS_REG_T0, 0x0F0F0F0F);
    push_mov_imm_i32(MIPS_REG_T1, 0xF0F0F0F0);
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T0, MIPS_REG_T1, MIPS_REG_T2, 0, MIPS_SPECIAL_AND));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T0, MIPS_REG_T1, MIPS_REG_T3, 0, MIPS_SPECIAL_OR));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T0, MIPS_REG_T1, MIPS_REG_T4, 0, MIPS_SPECIAL_XOR));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T0, MIPS_REG_T1, MIPS_REG_T5, 0, MIPS_SPECIAL_NOR));
    run_program();

    REQUIRE(r5900.gpr[MIPS_REG_T2].u32 == 0x00000000);
    REQUIRE(r5900.gpr[MIPS_REG_T3].u32 == 0xFFFFFFFF);
    REQUIRE(r5900.gpr[MIPS_REG_T4].u32 == 0xFFFFFFFF);
    REQUIRE(r5900.gpr[MIPS_REG_T5].u32 == 0x00000000);
}

TEST_CASE_METHOD(R5900ISATestFixture, "R5900 IALU signed and unsigned compares", "[epcs2_r5900_isa_ialu_test]") {
    push_mov_imm_i32(MIPS_REG_T0, -5);
    push_mov_imm_i32(MIPS_REG_T1, 4);
    push_mov_imm_i32(MIPS_REG_T2, 5);
    push_mov_imm_i32(MIPS_REG_T3, 4);
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T0, MIPS_REG_T1, MIPS_REG_T4, 0, MIPS_SPECIAL_SLT));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T2, MIPS_REG_T3, MIPS_REG_T5, 0, MIPS_SPECIAL_SLT));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T0, MIPS_REG_T1, MIPS_REG_T6, 0, MIPS_SPECIAL_SLTU));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T1, MIPS_REG_T2, MIPS_REG_T7, 0, MIPS_SPECIAL_SLTU));
    run_program();

    REQUIRE(r5900.gpr[MIPS_REG_T4].u32 == 1);
    REQUIRE(r5900.gpr[MIPS_REG_T5].u32 == 0);
    REQUIRE(r5900.gpr[MIPS_REG_T6].u32 == 0);
    REQUIRE(r5900.gpr[MIPS_REG_T7].u32 == 1);
}

TEST_CASE_METHOD(R5900ISATestFixture, "R5900 IALU shifts", "[epcs2_r5900_isa_ialu_test]") {
    push_mov_imm_i32(MIPS_REG_T0, 0x00000003);
    push_mov_imm_i32(MIPS_REG_T1, 0x80000000);
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T0, MIPS_REG_T0, MIPS_REG_T2, 2, MIPS_SPECIAL_SLL));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T0, MIPS_REG_T0, MIPS_REG_T3, 2, MIPS_SPECIAL_SRL));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T1, MIPS_REG_T1, MIPS_REG_T4, 1, MIPS_SPECIAL_SRA));
    run_program();

    REQUIRE(r5900.gpr[MIPS_REG_T2].u32 == 0x0000000C);
    REQUIRE(r5900.gpr[MIPS_REG_T3].u32 == 0x00000000);
    REQUIRE(r5900.gpr[MIPS_REG_T4].i32 == 0xC0000000);
}

TEST_CASE_METHOD(R5900ISATestFixture, "R5900 IALU variable shifts", "[epcs2_r5900_isa_ialu_test]") {
    push_mov_imm_i32(MIPS_REG_T0, 0x00000003);
    push_mov_imm_i32(MIPS_REG_T1, 0x00000001);
    push_mov_imm_i32(MIPS_REG_T2, 0x80000000);
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T1, MIPS_REG_T0, MIPS_REG_T3, 0, MIPS_SPECIAL_SLLV));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T1, MIPS_REG_T0, MIPS_REG_T4, 0, MIPS_SPECIAL_SRLV));
    push_instruction(encode_rtype(MIPS_OP_SPECIAL, MIPS_REG_T1, MIPS_REG_T2, MIPS_REG_T5, 0, MIPS_SPECIAL_SRAV));
    run_program();

    REQUIRE(r5900.gpr[MIPS_REG_T3].u32 == 0x00000006);
    REQUIRE(r5900.gpr[MIPS_REG_T4].u32 == 0x00000001);
    REQUIRE(r5900.gpr[MIPS_REG_T5].i32 == 0xC0000000);
}