#pragma once

#include "qx/V3xLibqasmInterface.hpp"
#include "qx/SimulationError.hpp"
#include "v3x/cqasm-semantic-gen.hpp"

#include <cstdint>  // size_t
#include <string>
#include <unordered_map>
#include <vector>


namespace qx {

/*
 * RegisterManager keeps track of a (virtual) qubit register and a (virtual) bit register.
 * I.e., an array of consecutive qubits/bits, and the mappings between the (logical) qubit/bit variable names,
 * as used in an input cQASM program, and the (virtual) qubit/bit register.
 *
 * For example, given an input program that defines 'qubit[3] q':
 * - variable 'q' is mapped to qubits 0 to 2 in the qubit register, and
 * - positions 0 to 2 in the qubit register are mapped to variable 'q'.
 *
 * The mapping of qubit/bit variable names to positions in the qubit/bit register is an implementation detail,
 * i.e., it is not guaranteed that qubit/bit register indices are assigned to qubit/bit variable names in the order
 * these variables are defined in the input program.
 */


using VariableName = std::string;


struct Range {
    std::size_t first;
    std::size_t size;
};


struct RegisterManagerError : public SimulationError {
    explicit RegisterManagerError(const std::string &message);
};


class Register {
    using VariableNameToRangeMapT = std::unordered_map<VariableName, Range>;
    using IndexToVariableNameMapT = std::vector<VariableName>;
private:
    std::size_t register_size_;
    VariableNameToRangeMapT variable_name_to_range_;
    IndexToVariableNameMapT index_to_variable_name_;
public:
    Register(const V3OneProgram &program, auto &&is_of_type, std::size_t max_register_size);
    virtual ~Register() = 0;
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] virtual Range at(const VariableName &name) const;
    [[nodiscard]] virtual VariableName at(const std::size_t &index) const;
};


class QubitRegister : public Register {
public:
    explicit QubitRegister(const V3OneProgram &program);
    virtual ~QubitRegister() override;
};


class BitRegister : public Register {
public:
    explicit BitRegister(const V3OneProgram &program);
    virtual ~BitRegister() override;
};


class RegisterManager {
    QubitRegister qubit_register_;
    BitRegister bit_register_;
public:
    RegisterManager(const RegisterManager&) = delete;
    RegisterManager(RegisterManager&&) noexcept = delete;
    RegisterManager& operator=(const RegisterManager&) = default;
    RegisterManager& operator=(RegisterManager&&) noexcept = default;
public:
    explicit RegisterManager(const V3OneProgram &program);
    [[nodiscard]] std::size_t get_qubit_register_size() const;
    [[nodiscard]] std::size_t get_bit_register_size() const;
    [[nodiscard]] Range get_qubit_range(const VariableName &name) const;
    [[nodiscard]] Range get_bit_range(const VariableName &name) const;
    [[nodiscard]] VariableName get_qubit_variable_name(const std::size_t &index) const;
    [[nodiscard]] VariableName get_bit_variable_name(const std::size_t &index) const;
};

}  // namespace qx
