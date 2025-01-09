#include "qx/error_models.hpp"

#include <gtest/gtest.h>

#include "qx/random.hpp"

namespace qx::error_models {

using namespace std::complex_literals;

class ErrorModelsTest : public ::testing::Test {
protected:
    void SetUp() override { random::seed(123); }

    void check_state(const std::map<core::BasisVector, std::complex<double>>& expected) {
        state.for_each([&expected](auto const& kv) {
            auto const& [basis_vector, sparse_complex] = kv;
            ASSERT_EQ(expected.count(basis_vector), 1);
            EXPECT_EQ(expected.at(basis_vector), sparse_complex.value);
        });
    }

    template <typename ErrorModel>
    void add_error(ErrorModel& error_model) {
        error_model.add_error(state);
    }

private:
    core::QuantumState state{ 3, 3 };  // using a mock or a TestQuantumState would be beneficial here
};

TEST_F(ErrorModelsTest, depolarizing_channel__probability_1) {
    DepolarizingChannel const channel(1.);

    add_error(channel);
    // X is applied to qubit 1
    check_state({
        { core::BasisVector{ std::string{ "010" } }, 1. + 0.i }
    });

    add_error(channel);
    // Z is applied to qubit 2
    check_state({
        { core::BasisVector{ std::string{ "010" } }, 1. + 0.i }
    });

    add_error(channel);
    // X is applied to qubit 0
    check_state({
        { core::BasisVector{ std::string{ "011" } }, 1. + 0.i }
    });
}

TEST_F(ErrorModelsTest, depolarizing_channel__probability_0) {
    DepolarizingChannel const channel(0.);

    add_error(channel);
    add_error(channel);
    add_error(channel);
    add_error(channel);

    check_state({
        { core::BasisVector{ std::string{ "000" } }, 1. + 0.i }
    });
}

}  // namespace qx::error_models
