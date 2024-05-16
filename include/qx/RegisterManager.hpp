#pragma once

#include "qx/V3xLibqasmInterface.hpp"
#include "v3x/cqasm-semantic-gen.hpp"

#include <cstdint>  // size_t
#include <string>
#include <unordered_map>
#include <vector>


namespace qx {

using VariableName = std::string;

struct QubitRange {
    std::size_t first;
    std::size_t size;
};

/*
 * RegisterManager keeps track of a (virtual) qubit register, i.e., an array of consecutive qubits,
 * and the mappings between the (logical) qubit variable names, as used in an input cQASM program,
 * and the (virtual) qubit register.
 *
 * For example, given an input program that defines 'qubit[3] q':
 * - variable 'q' is mapped to qubits 0 to 2 in the qubit register, and
 * - positions 0 to 2 in the qubit register are mapped to variable 'q'.
 *
 * The mapping of qubit variable names to positions in the qubit register is an implementation detail,
 * i.e., it is not guaranteed that qubit register indices are assigned to qubit variable names in the order
 * these variables are defined in the input program.
 */
class RegisterManager {
    using VariableNameToQubitRangeMapT = std::unordered_map<VariableName, QubitRange>;
    using QubitIndexToVariableNameMapT = std::vector<VariableName>;
private:
    V3Program program_;
    std::size_t qubit_register_size_;
    VariableNameToQubitRangeMapT variable_name_to_qubit_range_;
    QubitIndexToVariableNameMapT qubit_index_to_variable_name_;
public:
    RegisterManager(const RegisterManager&) = delete;
    RegisterManager(RegisterManager&&) noexcept = delete;
    RegisterManager& operator=(const RegisterManager&) = default;
    RegisterManager& operator=(RegisterManager&&) noexcept = default;
public:
    explicit RegisterManager(const V3Program &program);
    [[nodiscard]] std::size_t get_qubit_register_size() const;
    [[nodiscard]] QubitRange get_qubit_range(const VariableName &name) const;
    [[nodiscard]] VariableName get_variable_name(std::size_t index) const;
};

}  // namespace qx
