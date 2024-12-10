#pragma once

#include "qx/quantum_state.hpp"

#include <variant>


namespace qx {

class Circuit;

namespace error_models {

class DepolarizingChannel {
public:
    explicit DepolarizingChannel(double p);
    void add_error(qx::core::QuantumState &quantum_state) const;

private:
    double probability;
};

using ErrorModel = std::variant<DepolarizingChannel, std::monostate>;

template <typename ErrorModelDef>
ErrorModel get_error_model(ErrorModelDef error_model_def) {
    if (error_model_def.empty()) {
        return std::monostate();
    }

    if (error_model_def->name != "depolarizing_channel") {
        throw std::runtime_error{ "unknown error model" };
    }

    return DepolarizingChannel{ error_model_def->parameters[0]->as_const_real()->value };
}

}  // namespace error_models
}  // namespace qx
