#pragma once

#include <fmt/ostream.h>

#include <cstdint>  // size_t
#include <memory>  // shared_ptr
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "libqasm/v3x/semantic_generated.hpp"
#include "qx/cqasm_v3x.hpp"
#include "qx/simulation_error.hpp"

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

using Index = std::size_t;

struct Range {
    Index first;
    Index size;
};

//----------------------//
// RegisterManagerError //
//----------------------//

struct RegisterManagerError : public SimulationError {
    explicit RegisterManagerError(const std::string& message);
};

using VariableNameToRangeMapT = std::unordered_map<VariableName, Range>;
using IndexToVariableNameMapT = std::vector<VariableName>;

//----------//
// Register //
//----------//

class Register {
    std::size_t register_size_;
    VariableNameToRangeMapT variable_name_to_range_;
    IndexToVariableNameMapT index_to_variable_name_;

public:
    Register(const TreeOne<CqasmV3xProgram>& program, auto&& is_of_type, std::size_t max_register_size);
    virtual ~Register() = 0;
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] virtual Range at(const VariableName& name) const;
    [[nodiscard]] virtual Index at(const VariableName& name, const std::optional<Index>& sub_index) const;
    [[nodiscard]] virtual VariableName at(const std::size_t& index) const;
    [[nodiscard]] virtual std::string to_string() const;
};

//---------------//
// QubitRegister //
//---------------//

class QubitRegister : public Register {
public:
    explicit QubitRegister(const TreeOne<CqasmV3xProgram>& program);
    ~QubitRegister() override;
};

//-------------//
// BitRegister //
//-------------//

class BitRegister : public Register {
public:
    explicit BitRegister(const TreeOne<CqasmV3xProgram>& program);
    ~BitRegister() override;
};

//-----------------//
// RegisterManager //
//-----------------//

class RegisterManager {
    std::shared_ptr<QubitRegister> qubit_register_;
    std::shared_ptr<BitRegister> bit_register_;

    RegisterManager() = default;
    [[nodiscard]] static RegisterManager& get_instance_impl();

public:
    RegisterManager(const RegisterManager&) = delete;
    RegisterManager(RegisterManager&&) noexcept = delete;
    RegisterManager& operator=(const RegisterManager&) = default;
    RegisterManager& operator=(RegisterManager&&) noexcept = default;

public:
    static void create_instance(const TreeOne<CqasmV3xProgram>& program);
    [[nodiscard]] static RegisterManager& get_instance();
    [[nodiscard]] std::size_t get_qubit_register_size() const;
    [[nodiscard]] std::size_t get_bit_register_size() const;
    [[nodiscard]] Range get_qubit_range(const VariableName& name) const;
    [[nodiscard]] Range get_bit_range(const VariableName& name) const;
    [[nodiscard]] Index get_qubit_index(const VariableName& name, const std::optional<Index>& sub_index) const;
    [[nodiscard]] Index get_bit_index(const VariableName& name, const std::optional<Index>& sub_index) const;
    [[nodiscard]] VariableName get_qubit_variable_name(const Index& index) const;
    [[nodiscard]] VariableName get_bit_variable_name(const Index& index) const;
    [[nodiscard]] std::shared_ptr<QubitRegister> get_qubit_register() const;
    [[nodiscard]] std::shared_ptr<BitRegister> get_bit_register() const;
};

std::ostream& operator<<(std::ostream& os, Range const& range);
std::ostream& operator<<(std::ostream& os, QubitRegister const& qubit_register);
std::ostream& operator<<(std::ostream& os, BitRegister const& bit_register);

}  // namespace qx

template <>
struct fmt::formatter<qx::Range> : fmt::ostream_formatter {};
template <>
struct fmt::formatter<qx::QubitRegister> : fmt::ostream_formatter {};
template <>
struct fmt::formatter<qx::BitRegister> : fmt::ostream_formatter {};
