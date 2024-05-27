#pragma once

#include "qx/Core.hpp"

#include <variant>


namespace qx {

class Circuit;

namespace error_models {

class DepolarizingChannel {
public:
    explicit DepolarizingChannel(double p) : probability(p) {
        assert(0. <= p && p <= 1.);
    }

    void addError(qx::core::QuantumState &quantumState) const;

private:
    double probability = 0.;
};

using ErrorModel = std::variant<DepolarizingChannel, std::monostate>;

template <typename ErrorModelDef>
ErrorModel getErrorModel(ErrorModelDef errorModelDef) {
    if (errorModelDef.empty()) {
        return std::monostate();
    }

    if (errorModelDef->name != "depolarizing_channel") {
        throw std::runtime_error("Unknown error model!");
    }

    return DepolarizingChannel(
        errorModelDef->parameters[0]->as_const_real()->value);
}

}  // namespace error_models
}  // namespace qx