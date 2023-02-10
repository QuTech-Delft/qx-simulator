#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "qx/ErrorModels.hpp"
#include "doctest/doctest.h"
#include "qx/Random.hpp"

namespace qx {
namespace error_models {

using namespace std::complex_literals;

class ErrorModelsTest {
protected:
    void checkState(std::map<BasisVector, std::complex<double>> expected) {
        state.forEach([&expected](auto const &kv) {
            CAPTURE(kv.first.toString());
            CAPTURE(kv.second);
            REQUIRE_EQ(expected.count(kv.first), 1);
            CHECK_EQ(expected[kv.first], kv.second);
        });
    }

    template <typename ErrorModel> void addError(ErrorModel &errorModel) {
        errorModel.addError(state);
    }

private:
    core::QuantumState state{
        3}; // Using a mock or a TestQuantumState would be beneficial here.
};

TEST_CASE_FIXTURE(ErrorModelsTest, "Depolarizing channel") {
    random::seed(123);

    SUBCASE("Probability 1") {
        DepolarizingChannel const channel(1.);
        addError(channel);
        // X is applied to qubit 1.
        checkState({{{"010"}, 1. + 0.i}});

        addError(channel);
        // Z is applied to qubit 2.
        checkState({{{"010"}, 1. + 0.i}});

        addError(channel);
        // X is applied to qubit 0.
        checkState({{{"011"}, 1. + 0.i}});
    }

    SUBCASE("Probability 0") {
        DepolarizingChannel const channel(0.);

        addError(channel);
        addError(channel);
        addError(channel);
        addError(channel);

        checkState({{{"000"}, 1. + 0.i}});
    }
}

} // namespace error_models
} // namespace qx