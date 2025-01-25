#include "qx/operands_helper.hpp"
#include "qx/register_manager.hpp"

#include <gtest/gtest.h>

#include <vector>

namespace qx {

class OperandsHelperTest : public ::testing::Test {
protected:
    void SetUp() override {
        program_sp->variables = variables;
        RegisterManager::create_instance(program_sp);
    }

    // Types
    TreeOne<CqasmV3xTypeBase> qubit_1_type_sp = TreeOne<CqasmV3xTypeBase>{ std::make_shared<CqasmV3xQubitType>(1) };
    TreeOne<CqasmV3xTypeBase> qubit_2_type_sp = TreeOne<CqasmV3xTypeBase>{ std::make_shared<CqasmV3xQubitType>(2) };
    TreeOne<CqasmV3xTypeBase> qubit_6_type_sp = TreeOne<CqasmV3xTypeBase>{ std::make_shared<CqasmV3xQubitType>(6) };

    // Variables
    CqasmV3xVariable q = { "q", qubit_1_type_sp };  // virtual qubit index 0
    CqasmV3xVariable qq2 = { "qq2", qubit_2_type_sp };  // 1, 2
    CqasmV3xVariable rr2 = { "rr2", qubit_2_type_sp };  // 3, 4
    CqasmV3xVariable qq6 = { "qq6", qubit_6_type_sp };  // 5, 6, 7, 8, 9, 10
    TreeOne<CqasmV3xVariable> q_sp = TreeOne<CqasmV3xVariable>{ std::make_shared<CqasmV3xVariable>(q) };
    TreeOne<CqasmV3xVariable> qq2_sp = TreeOne<CqasmV3xVariable>{ std::make_shared<CqasmV3xVariable>(qq2) };
    TreeOne<CqasmV3xVariable> rr2_sp = TreeOne<CqasmV3xVariable>{ std::make_shared<CqasmV3xVariable>(rr2) };
    TreeOne<CqasmV3xVariable> qq6_sp = TreeOne<CqasmV3xVariable>{ std::make_shared<CqasmV3xVariable>(qq6) };
    CqasmV3xVariableLink q_link = CqasmV3xMaybe<CqasmV3xVariable>{ q_sp };
    CqasmV3xVariableLink qq2_link = CqasmV3xMaybe<CqasmV3xVariable>{ qq2_sp };
    CqasmV3xVariableLink rr2_link = CqasmV3xMaybe<CqasmV3xVariable>{ rr2_sp };
    CqasmV3xVariableLink qq6_link = CqasmV3xMaybe<CqasmV3xVariable>{ qq6_sp };

    // Indices
    // Notice that, for testing purposes, these indices can be used both as logical and virtual
    // The functions in operands_helper start by managing CqasmV3xOperands with logical indices,
    // but then go on to manage CqasmV3xSgmqGroupsIndices with virtual indices
    TreeOne<CqasmV3xConstInt> index_0_sp = TreeOne<CqasmV3xConstInt>{ std::make_shared<CqasmV3xConstInt>( 0 ) };
    TreeOne<CqasmV3xConstInt> index_1_sp = TreeOne<CqasmV3xConstInt>{ std::make_shared<CqasmV3xConstInt>( 1 ) };
    TreeOne<CqasmV3xConstInt> index_2_sp = TreeOne<CqasmV3xConstInt>{ std::make_shared<CqasmV3xConstInt>( 2 ) };
    TreeOne<CqasmV3xConstInt> index_3_sp = TreeOne<CqasmV3xConstInt>{ std::make_shared<CqasmV3xConstInt>( 3 ) };
    TreeOne<CqasmV3xConstInt> index_4_sp = TreeOne<CqasmV3xConstInt>{ std::make_shared<CqasmV3xConstInt>( 4 ) };
    TreeOne<CqasmV3xConstInt> index_5_sp = TreeOne<CqasmV3xConstInt>{ std::make_shared<CqasmV3xConstInt>( 5 ) };
    TreeOne<CqasmV3xConstInt> index_6_sp = TreeOne<CqasmV3xConstInt>{ std::make_shared<CqasmV3xConstInt>( 6 ) };
    TreeOne<CqasmV3xConstInt> index_7_sp = TreeOne<CqasmV3xConstInt>{ std::make_shared<CqasmV3xConstInt>( 7 ) };
    TreeOne<CqasmV3xConstInt> index_8_sp = TreeOne<CqasmV3xConstInt>{ std::make_shared<CqasmV3xConstInt>( 8 ) };
    TreeOne<CqasmV3xConstInt> index_9_sp = TreeOne<CqasmV3xConstInt>{ std::make_shared<CqasmV3xConstInt>( 9 ) };
    TreeOne<CqasmV3xConstInt> index_10_sp = TreeOne<CqasmV3xConstInt>{ std::make_shared<CqasmV3xConstInt>( 10 ) };

    // Index groups
    CqasmV3xIndices rr2_1 = CqasmV3xMany<CqasmV3xConstInt>{ index_1_sp };
    CqasmV3xIndices qq6_0_1_2 = CqasmV3xMany<CqasmV3xConstInt>{ index_0_sp, index_1_sp, index_2_sp };
    CqasmV3xIndices qq6_3_4_5 = CqasmV3xMany<CqasmV3xConstInt>{ index_3_sp, index_4_sp, index_5_sp };

    // Values
    //
    // Variable references
    CqasmV3xVariableRef q_variable_ref = { q_link };  // q
    CqasmV3xVariableRef qq2_variable_ref = { qq2_link };  // qq2
    CqasmV3xVariableRef rr2_variable_ref = { rr2_link };  // rr2
    // Index references (logical)
    CqasmV3xIndexRef rr2_1_index_ref = { rr2_link, rr2_1 };  // rr2[1]
    CqasmV3xIndexRef qq6_0_1_2_index_ref = { qq6_link, qq6_0_1_2 };  // qq6[0, 1, 2]
    CqasmV3xIndexRef qq6_3_4_5_index_ref = { qq6_link, qq6_3_4_5 };  // qq6[3, 4, 5]

    TreeOne<CqasmV3xVariableRef> q_variable_ref_sp = TreeOne<CqasmV3xVariableRef>{ std::make_shared<CqasmV3xVariableRef>(q_variable_ref) };
    TreeOne<CqasmV3xVariableRef> qq2_variable_ref_sp = TreeOne<CqasmV3xVariableRef>{ std::make_shared<CqasmV3xVariableRef>(qq2_variable_ref) };
    TreeOne<CqasmV3xVariableRef> rr2_variable_ref_sp = TreeOne<CqasmV3xVariableRef>{ std::make_shared<CqasmV3xVariableRef>(rr2_variable_ref) };
    TreeOne<CqasmV3xIndexRef> rr2_1_index_ref_sp = TreeOne<CqasmV3xIndexRef>{ std::make_shared<CqasmV3xIndexRef>(rr2_1_index_ref) };
    TreeOne<CqasmV3xIndexRef> qq6_0_1_2_index_ref_sp = TreeOne<CqasmV3xIndexRef>{ std::make_shared<CqasmV3xIndexRef>(qq6_0_1_2_index_ref) };
    TreeOne<CqasmV3xIndexRef> qq6_3_4_5_index_ref_sp = TreeOne<CqasmV3xIndexRef>{ std::make_shared<CqasmV3xIndexRef>(qq6_3_4_5_index_ref) };

    // cQASM v3x operands
    CqasmV3xOperands ops_qq2__rr2 = CqasmV3xAny<CqasmV3xValueBase>{ qq2_variable_ref_sp, rr2_variable_ref_sp };
    CqasmV3xOperands ops_q__rr2_1 = CqasmV3xAny<CqasmV3xValueBase>{ q_variable_ref_sp, rr2_1_index_ref_sp };
    CqasmV3xOperands ops_qq6_0_1_2__qq6_3_4_5 = CqasmV3xAny<CqasmV3xValueBase>{ qq6_0_1_2_index_ref_sp, qq6_3_4_5_index_ref_sp };

    // cQASM v3x SGMQ groups of indices (logical in the variable name, virtual in the value)
    CqasmV3xOperandIndices op_q_indices = CqasmV3xAny<CqasmV3xConstInt>{ index_0_sp };
    CqasmV3xOperandIndices op_qq2_indices = CqasmV3xAny<CqasmV3xConstInt>{ index_1_sp, index_2_sp };
    CqasmV3xOperandIndices op_rr2_indices = CqasmV3xAny<CqasmV3xConstInt>{ index_3_sp, index_4_sp };
    CqasmV3xOperandIndices op_rr2_1_indices = CqasmV3xAny<CqasmV3xConstInt>{ index_4_sp };
    CqasmV3xOperandIndices op_qq6_0_1_2_indices = CqasmV3xAny<CqasmV3xConstInt>{ index_5_sp, index_6_sp, index_7_sp };
    CqasmV3xOperandIndices op_qq6_3_4_5_indices = CqasmV3xAny<CqasmV3xConstInt>{ index_8_sp, index_9_sp, index_10_sp };
    CqasmV3xSgmqGroupsIndices sgmqs_qq2__rr2 = std::vector<CqasmV3xOperandIndices>{ op_qq2_indices, op_rr2_indices };
    CqasmV3xSgmqGroupsIndices sgmqs_q__rr2_1 = std::vector<CqasmV3xOperandIndices>{ op_q_indices, op_rr2_1_indices };
    CqasmV3xSgmqGroupsIndices sgmqs_qq6_0_1_2__qq6_3_4_5 = std::vector<CqasmV3xOperandIndices>{ op_qq6_0_1_2_indices, op_qq6_3_4_5_indices };

    // cQASM v3x instructions indices (logical in the variable name, virtual in the value)
    CqasmV3xOperandIndices cqv3x_i_qq2_0__rr2_0_indices = CqasmV3xAny<CqasmV3xConstInt>{ index_1_sp, index_3_sp };
    CqasmV3xOperandIndices cqv3x_i_qq2_1__rr2_1_indices = CqasmV3xAny<CqasmV3xConstInt>{ index_2_sp, index_4_sp };
    CqasmV3xOperandIndices cqv3x_i_q__rr2_1_indices = CqasmV3xAny<CqasmV3xConstInt>{ index_0_sp, index_4_sp };
    CqasmV3xOperandIndices cqv3x_i_qq6_0__qq6_3_indices = CqasmV3xAny<CqasmV3xConstInt>{ index_5_sp, index_8_sp };
    CqasmV3xOperandIndices cqv3x_i_qq6_1__qq6_4_indices = CqasmV3xAny<CqasmV3xConstInt>{ index_6_sp, index_9_sp };
    CqasmV3xOperandIndices cqv3x_i_qq6_2__qq6_5_indices = CqasmV3xAny<CqasmV3xConstInt>{ index_7_sp, index_10_sp };
    CqasmV3xInstructionsIndices cqv3x_ii_qq2__rr2_indices = std::vector<CqasmV3xOperandIndices>{ cqv3x_i_qq2_0__rr2_0_indices, cqv3x_i_qq2_1__rr2_1_indices };
    CqasmV3xInstructionsIndices cqv3x_ii_q__rr2_1_indices = std::vector<CqasmV3xOperandIndices>{ cqv3x_i_q__rr2_1_indices };
    CqasmV3xInstructionsIndices cqv3x_ii_qq6_0_1_2__qq6_3_4_5_indices = std::vector<CqasmV3xOperandIndices>{ cqv3x_i_qq6_0__qq6_3_indices, cqv3x_i_qq6_1__qq6_4_indices, cqv3x_i_qq6_2__qq6_5_indices };

    // Instructions indices (logical in the variable name, virtual in the value)
    InstructionIndices i_qq2_0__rr2_0_indices = InstructionIndices{ core::Index{ 1 }, core::Index{ 3 } };
    InstructionIndices i_qq2_1__rr2_1_indices = InstructionIndices{ core::Index{ 2 }, core::Index{ 4 } };
    InstructionIndices i_q__rr2_1_indices = InstructionIndices{ core::Index{ 0 }, core::Index{ 4 } };
    InstructionIndices i_qq6_0__qq6_3_indices = InstructionIndices{ core::Index{ 5 }, core::Index{ 8 } };
    InstructionIndices i_qq6_1__qq6_4_indices = InstructionIndices{ core::Index{ 6 }, core::Index{ 9 } };
    InstructionIndices i_qq6_2__qq6_5_indices = InstructionIndices{ core::Index{ 7 }, core::Index{ 10 } };
    InstructionsIndices ii_qq2_0__rr2_0___qq2_1__rr2_1_indices = InstructionsIndices{ i_qq2_0__rr2_0_indices, i_qq2_1__rr2_1_indices };
    InstructionsIndices ii_q__rr2_1_indices = InstructionsIndices{ i_q__rr2_1_indices };
    InstructionsIndices ii_qq6_0_3__qq6_1_4__qq6_2_5_indices = InstructionsIndices{ i_qq6_0__qq6_3_indices, i_qq6_1__qq6_4_indices, i_qq6_2__qq6_5_indices };

    // Program
    CqasmV3xVariables variables = CqasmV3xAny<CqasmV3xVariable>{ q_sp, qq2_sp, rr2_sp, qq6_sp };
    CqasmV3xProgram program = CqasmV3xProgram{};
    TreeOne<CqasmV3xProgram> program_sp = TreeOne<CqasmV3xProgram>{ std::make_shared<CqasmV3xProgram>(program) };
};

TEST_F(OperandsHelperTest, get_instructions_indices) {
    EXPECT_EQ(ii_qq2_0__rr2_0___qq2_1__rr2_1_indices, get_instructions_indices(ops_qq2__rr2));  // two variables
    EXPECT_EQ(ii_q__rr2_1_indices, get_instructions_indices(ops_q__rr2_1));  // variable and index
    EXPECT_EQ(ii_qq6_0_3__qq6_1_4__qq6_2_5_indices, get_instructions_indices(ops_qq6_0_1_2__qq6_3_4_5));  // two indices
}

TEST_F(OperandsHelperTest, get_cqasm_v3x_sgmq_groups_indices) {
    const auto& ret_1 = get_cqasm_v3x_sgmq_groups_indices(ops_qq2__rr2);
    ASSERT_EQ(ret_1.size(), 2);
    EXPECT_TRUE(ret_1[0].equals(op_qq2_indices));
    EXPECT_TRUE(ret_1[1].equals(op_rr2_indices));

    const auto& ret_2 = get_cqasm_v3x_sgmq_groups_indices(ops_q__rr2_1);
    ASSERT_EQ(ret_2.size(), 2);
    EXPECT_TRUE(ret_2[0].equals(op_q_indices));
    EXPECT_TRUE(ret_2[1].equals(op_rr2_1_indices));

    const auto& ret_3 = get_cqasm_v3x_sgmq_groups_indices(ops_qq6_0_1_2__qq6_3_4_5);
    ASSERT_EQ(ret_3.size(), 2);
    EXPECT_TRUE(ret_3[0].equals(op_qq6_0_1_2_indices));
    EXPECT_TRUE(ret_3[1].equals(op_qq6_3_4_5_indices));
}

TEST_F(OperandsHelperTest, get_cqasm_v3x_sgmq_group_indices) {
    EXPECT_TRUE(op_qq2_indices.equals(get_cqasm_v3x_sgmq_group_indices(qq2_variable_ref)));
    EXPECT_TRUE(op_rr2_indices.equals(get_cqasm_v3x_sgmq_group_indices(rr2_variable_ref)));

    EXPECT_TRUE(op_q_indices.equals(get_cqasm_v3x_sgmq_group_indices(q_variable_ref)));
    EXPECT_TRUE(op_rr2_1_indices.equals(get_cqasm_v3x_sgmq_group_indices(rr2_1_index_ref)));

    EXPECT_TRUE(op_qq6_0_1_2_indices.equals(get_cqasm_v3x_sgmq_group_indices(qq6_0_1_2_index_ref)));
    EXPECT_TRUE(op_qq6_3_4_5_indices.equals(get_cqasm_v3x_sgmq_group_indices(qq6_3_4_5_index_ref)));
}

TEST_F(OperandsHelperTest, to_cqasm_v3x_instructions_indices) {
    const auto& ret_1 = to_cqasm_v3x_instructions_indices(sgmqs_qq2__rr2);
    ASSERT_EQ(ret_1.size(), 2);
    EXPECT_TRUE(ret_1[0].equals(cqv3x_i_qq2_0__rr2_0_indices));
    EXPECT_TRUE(ret_1[1].equals(cqv3x_i_qq2_1__rr2_1_indices));

    const auto& ret_2 = to_cqasm_v3x_instructions_indices(sgmqs_q__rr2_1);
    ASSERT_EQ(ret_2.size(), 1);
    EXPECT_TRUE(ret_2[0].equals(cqv3x_i_q__rr2_1_indices));

    const auto& ret_3 =  to_cqasm_v3x_instructions_indices(sgmqs_qq6_0_1_2__qq6_3_4_5);
    ASSERT_EQ(ret_3.size(), 3);
    EXPECT_TRUE(ret_3[0].equals(cqv3x_i_qq6_0__qq6_3_indices));
    EXPECT_TRUE(ret_3[1].equals(cqv3x_i_qq6_1__qq6_4_indices));
    EXPECT_TRUE(ret_3[2].equals(cqv3x_i_qq6_2__qq6_5_indices));
}

TEST_F(OperandsHelperTest, to_instructions_indices) {
    EXPECT_EQ(ii_qq2_0__rr2_0___qq2_1__rr2_1_indices, to_instructions_indices(cqv3x_ii_qq2__rr2_indices));
    EXPECT_EQ(ii_q__rr2_1_indices, to_instructions_indices(cqv3x_ii_q__rr2_1_indices));
    EXPECT_EQ(ii_qq6_0_3__qq6_1_4__qq6_2_5_indices, to_instructions_indices(cqv3x_ii_qq6_0_1_2__qq6_3_4_5_indices));
}

TEST_F(OperandsHelperTest, to_instruction_indices) {
    EXPECT_EQ(i_qq2_0__rr2_0_indices, to_instruction_indices(cqv3x_i_qq2_0__rr2_0_indices));
    EXPECT_EQ(i_qq2_1__rr2_1_indices, to_instruction_indices(cqv3x_i_qq2_1__rr2_1_indices));

    EXPECT_EQ(i_q__rr2_1_indices, to_instruction_indices(cqv3x_i_q__rr2_1_indices));

    EXPECT_EQ(i_qq6_0__qq6_3_indices, to_instruction_indices(cqv3x_i_qq6_0__qq6_3_indices));
    EXPECT_EQ(i_qq6_1__qq6_4_indices, to_instruction_indices(cqv3x_i_qq6_1__qq6_4_indices));
    EXPECT_EQ(i_qq6_2__qq6_5_indices, to_instruction_indices(cqv3x_i_qq6_2__qq6_5_indices));
}

}  // namespace qx
