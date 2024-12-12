#include "qx/compile_time_configuration.hpp"
#include "qx/cqasm_v3x.hpp"
#include "qx/register_manager.hpp"

#include <fmt/format.h>
#include <gmock/gmock.h>  // ThrowsMessage
#include <gtest/gtest.h>
#include <stdexcept>  // runtime_error


namespace qx {

class RegisterManagerTest : public ::testing::Test {
protected:
    void SetUp() override {}

    CqasmV3xApiVersion api_version = CqasmV3xApiVersion{ "3.0" };
    TreeOne<CqasmV3xVersion> version = cqasm_tree::make<CqasmV3xVersion>("3.0");
    TreeOne<CqasmV3xBlock> empty_block = cqasm_tree::make<CqasmV3xBlock>();
    TreeOne<CqasmV3xVariable> qubit_register_of_size_2 = cqasm_tree::make<CqasmV3xVariable>(
        "qr_of_size_2", cqasm_tree::make<CqasmV3xQubitType>(2));
    TreeOne<CqasmV3xVariable> qubit_register_of_size_4 = cqasm_tree::make<CqasmV3xVariable>(
        "qr_of_size_4", cqasm_tree::make<CqasmV3xQubitType>(4));
    TreeOne<CqasmV3xVariable> too_long_qubit_register = cqasm_tree::make<CqasmV3xVariable>(
        "too_long_qr", cqasm_tree::make<CqasmV3xQubitType>(qx::config::MAX_QUBIT_NUMBER + 1));
    TreeOne<CqasmV3xVariable> too_long_bit_register = cqasm_tree::make<CqasmV3xVariable>(
        "too_long_br", cqasm_tree::make<CqasmV3xBitType>(qx::config::MAX_BIT_NUMBER + 1));
    TreeOne<CqasmV3xProgram> null_program = TreeOne<CqasmV3xProgram>{};
    TreeOne<CqasmV3xProgram> program_with_2_qubits = cqasm_tree::make<CqasmV3xProgram>(
        api_version, version, empty_block, CqasmV3xVariables{ qubit_register_of_size_2 }
    );
    TreeOne<CqasmV3xProgram> program_with_4_qubits = cqasm_tree::make<CqasmV3xProgram>(
        api_version, version, empty_block, CqasmV3xVariables{ qubit_register_of_size_4 }
    );
    TreeOne<CqasmV3xProgram> program_with_too_long_qubit_register = cqasm_tree::make<CqasmV3xProgram>(
        api_version, version, empty_block, CqasmV3xVariables{ too_long_qubit_register }
    );
    TreeOne<CqasmV3xProgram> program_with_too_long_bit_register = cqasm_tree::make<CqasmV3xProgram>(
        api_version, version, empty_block, CqasmV3xVariables{ too_long_bit_register }
    );
};

TEST_F(RegisterManagerTest, qubit_register_size_exceeds_maximum_allowed) {
    EXPECT_THAT(
        [this]() {
            [[maybe_unused]] auto qubit_register = QubitRegister{ program_with_too_long_qubit_register };
        },
        ::testing::ThrowsMessage<std::runtime_error>(
            fmt::format("register size exceeds maximum allowed: {} > {}",
                        qx::config::MAX_QUBIT_NUMBER + 1, qx::config::MAX_QUBIT_NUMBER)
        )
    );
}

TEST_F(RegisterManagerTest, bit_register_size_exceeds_maximum_allowed) {
    EXPECT_THAT(
        [this]() {
            [[maybe_unused]] auto bit_register = BitRegister{ program_with_too_long_bit_register };
        },
        ::testing::ThrowsMessage<std::runtime_error>(
            fmt::format("register size exceeds maximum allowed: {} > {}",
                        qx::config::MAX_BIT_NUMBER + 1, qx::config::MAX_BIT_NUMBER)
        )
    );
}

TEST_F(RegisterManagerTest, create_instance_with_null_program) {
    EXPECT_THAT(
        [this]() {
            RegisterManager::create_instance(null_program);
            [[maybe_unused]] const auto &unused = RegisterManager::get_instance();
        },
        ::testing::ThrowsMessage<std::runtime_error>("null pointer to program")
    );
}

TEST_F(RegisterManagerTest, get_instance_before_create_instance) {
    EXPECT_THAT(
        []() { [[maybe_unused]] const auto &unused = RegisterManager::get_instance(); },
        ::testing::ThrowsMessage<std::runtime_error>("uninitialized register manager")
    );
}

TEST_F(RegisterManagerTest, get_instance_after_two_calls_to_create_instance) {
    RegisterManager::create_instance(program_with_2_qubits);
    RegisterManager::create_instance(program_with_4_qubits);
    const auto &register_manager = RegisterManager::get_instance();
    EXPECT_EQ(register_manager.get_qubit_register_size(), 4);
}

}  // namespace qx
