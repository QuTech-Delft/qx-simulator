#include "qx/CompileTimeConfiguration.hpp"
#include "qx/RegisterManager.hpp"
#include "qx/SimulationError.hpp"

#include <algorithm>  // fill
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>
#include <stdexcept>  // runtime_error


namespace qx {

struct RegisterManagerError : public SimulationError {
    explicit RegisterManagerError(const std::string &message)
        : SimulationError{ message }
    {}
};

RegisterManager::RegisterManager(const V3Program &program) {
    auto is_qubit_variable = [](const V3Variable &variable) {
        return variable->typ->as_qubit() || variable->typ->as_qubit_array(); };
    auto &&qubit_variable_sizes = program->variables.get_vec()
        | ranges::views::filter([&](const V3Variable &variable) { return is_qubit_variable(variable); })
        | ranges::views::transform([](const V3Variable &variable) { return v3_types::size_of(variable->typ); });
    qubit_register_size_ = ranges::accumulate(qubit_variable_sizes, size_t{});

    if (qubit_register_size_ > config::MAX_QUBIT_NUMBER) {
        throw SimulationError{ "Cannot run that many qubits in this version of QX-simulator" };
    }

    variable_name_to_qubit_range_.reserve(qubit_register_size_);
    qubit_index_to_variable_name_.resize(qubit_register_size_);

    auto current_qubit_index = size_t{};
    for (const auto &variable: program->variables) {
        const auto &variable_size = static_cast<size_t>(cqasm::v3x::types::size_of(variable->typ));
        variable_name_to_qubit_range_[variable->name] = QubitRange{ current_qubit_index, variable_size };
        std::fill(qubit_index_to_variable_name_.begin() + static_cast<long>(current_qubit_index),
                  qubit_index_to_variable_name_.begin() + static_cast<long>(current_qubit_index + variable_size),
                  variable->name);
    };
}

[[nodiscard]] std::size_t RegisterManager::get_qubit_register_size() const {
    return qubit_register_size_;
}

[[nodiscard]] QubitRange RegisterManager::get_qubit_range(const VariableName &name) const {
    return variable_name_to_qubit_range_.at(name);
}

[[nodiscard]] VariableName RegisterManager::get_variable_name(std::size_t index) const {
    return qubit_index_to_variable_name_.at(index);
}

}  // namespace qx
