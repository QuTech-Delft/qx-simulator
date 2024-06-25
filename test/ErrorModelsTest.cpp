#include "qx/ErrorModels.hpp"
#include "qx/Random.hpp"

#include <gtest/gtest.h>


namespace qx::error_models {

using namespace std::complex_literals;

class ErrorModelsTest : public ::testing::Test {
protected:
    void SetUp() override {
        random::seed(123);
    }

    void checkState(const std::map<core::BasisVector, std::complex<double>> &expected) {
        state.forEach([&expected](auto const &kv) {
            auto const &[basisVector, sparseComplex] = kv;
            ASSERT_EQ(expected.count(basisVector), 1);
            EXPECT_EQ(expected.at(basisVector), sparseComplex.value);
        });
    }

    template <typename ErrorModel> void addError(ErrorModel &errorModel) {
        errorModel.addError(state);
    }

private:
    core::QuantumState state{ 3 }; // Using a mock or a TestQuantumState would be beneficial here.
};

TEST_F(ErrorModelsTest, depolarizing_channel__probability_1) {
    DepolarizingChannel const channel(1.);
    addError(channel);
    // X is applied to qubit 1.
    checkState({{core::BasisVector{"010"}, 1. + 0.i}});

    addError(channel);
    // Z is applied to qubit 2.
    checkState({{core::BasisVector{"010"}, 1. + 0.i}});

    addError(channel);
    // X is applied to qubit 0.
    checkState({{core::BasisVector{"011"}, 1. + 0.i}});
}

TEST_F(ErrorModelsTest, depolarizing_channel__probability_0) {
    DepolarizingChannel const channel(0.);

    addError(channel);
    addError(channel);
    addError(channel);
    addError(channel);

    checkState({{core::BasisVector{"000"}, 1. + 0.i}});
}

}  // namespace qx::error_models
