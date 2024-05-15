#include "qx/CompileTimeConfiguration.hpp"
#include "qx/RegisterManager.hpp"
#include "qx/SimulationError.hpp"

#include <algorithm>  // fill
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>


namespace qx {

explicit RegisterManager::RegisterManager(const V3Program &program) {
    auto is_qubit_variable = [](const V3Variable &variable) {
        return variable.typ->as_qubit() || variable.typ->as_qubit_array(); };

    const auto &qubit_variables = program->variables
        | ranges::views::filter([&](const V3Variable &variable) { return is_qubit_variable(variable); })
        | ranges::views::transform([](const V3Variable &variable) { return variable.typ; });

    qubit_register_size_ = ranges::accumulate(qubit_variables, std::size_t{},
        [](const V3Type &type) { return cqasm::v3x::types::size_of(type); });

    if (qubit_register_size_ > config::MAX_QUBIT_NUMBER) {
        throw SimulationError{ "Cannot run that many qubits in this version of QX-simulator" };
    }

    variable_name_to_qubit_range_.reserve(qubit_register_size_);
    qubit_index_to_variable_name_.resize(qubit_register_size_);

    auto current_qubit_index = std::size_t{};
    for (const auto &variable: program->variables) {
        const auto &variable_size = cqasm::v3x::types::size_of(variable->typ);
        variable_name_to_qubit_range_[variable->name] = QubitRange{ current_qubit_index, variable_size };
        std::fill(qubit_index_to_variable_name_.begin() + current_qubit_index,
                  qubit_index_to_variable_name_.begin() + current_qubit_index + variable_size,
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
