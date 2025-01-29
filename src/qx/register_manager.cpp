#include "qx/register_manager.hpp"

#include <fmt/format.h>

#include <algorithm>  // fill
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/transform.hpp>

#include "qx/compile_time_configuration.hpp"
#include "qx/cqasm_v3x.hpp"

namespace qx {

//----------------------//
// RegisterManagerError //
//----------------------//

RegisterManagerError::RegisterManagerError(const std::string& message)
: SimulationError{ message } {}

//----------//
// Register //
//----------//

Register::Register(const TreeOne<CqasmV3xProgram>& program, auto&& is_of_type, std::size_t max_register_size) {
    auto&& variables = program->variables.get_vec() |
        ranges::views::filter([&](const TreeOne<CqasmV3xVariable>& variable) { return is_of_type(*variable); });
    auto&& variable_sizes = variables | ranges::views::transform([](const TreeOne<CqasmV3xVariable>& variable) {
        return cqasm_v3x_types::size_of(variable->typ);
    });
    register_size_ = ranges::accumulate(variable_sizes, size_t{});

    if (register_size_ > max_register_size) {
        throw RegisterManagerError{ fmt::format(
            "register size exceeds maximum allowed: {} > {}", register_size_, max_register_size) };
    }

    variable_name_to_range_.reserve(register_size_);
    index_to_variable_name_.resize(register_size_);
    dirty_bitset_ = DirtyBitsetT{ register_size_ };

    auto current_index = size_t{};
    for (auto&& variable : variables) {
        const auto& variable_size = static_cast<size_t>(cqasm::v3x::types::size_of(variable->typ));
        variable_name_to_range_[variable->name] = Range{ current_index, variable_size };
        std::fill(index_to_variable_name_.begin() + static_cast<long>(current_index),
            index_to_variable_name_.begin() + static_cast<long>(current_index + variable_size),
            variable->name);
        current_index += variable_size;
    };
}

Register::~Register() = default;

[[nodiscard]] std::size_t Register::size() const {
    return register_size_;
}

[[nodiscard]] Range Register::at(const VariableName& name) const {
    return variable_name_to_range_.at(name);
}

[[nodiscard]] Index Register::at(const VariableName& name, const std::optional<Index>& sub_index) const {
    auto range = variable_name_to_range_.at(name);
    return range.first + sub_index.value_or(0);
}

[[nodiscard]] VariableName Register::at(const Index& index) const {
    return index_to_variable_name_.at(index);
}

[[nodiscard]] std::string Register::to_string() const {
    auto entries =
        ranges::accumulate(variable_name_to_range_, std::string{}, [first = true](auto total, const auto& kv) mutable {
            const auto& [variableName, range] = kv;
            total += fmt::format("{}{}: {}", first ? "" : ", ", variableName, range);
            first = false;
            return total;
        });
    return fmt::format("{{ {0} }}", entries);
}

[[nodiscard]] bool Register::is_dirty(const Index& index) const {
    return dirty_bitset_[index];
}

void Register::set_dirty(const Index& index) {
    assert(index < register_size_);
    dirty_bitset_[index] = true;
}

//---------------//
// QubitRegister //
//---------------//

QubitRegister::QubitRegister(const TreeOne<CqasmV3xProgram>& program)
: Register{ program, is_qubit_variable, config::MAX_QUBIT_NUMBER } {}

QubitRegister::~QubitRegister() = default;

//-----------------//
// RegisterManager //
//-----------------//

BitRegister::BitRegister(const TreeOne<CqasmV3xProgram>& program)
: Register{ program, is_bit_variable, config::MAX_BIT_NUMBER } {}

BitRegister::~BitRegister() = default;

//-----------------//
// RegisterManager //
//-----------------//

[[nodiscard]] /* static */ RegisterManager& RegisterManager::get_instance_impl() {
    static auto instance = RegisterManager{};
    return instance;
}

/* static */ void RegisterManager::create_instance(const TreeOne<CqasmV3xProgram>& program) {
    auto& instance = get_instance_impl();
    instance.qubit_register_ = std::make_shared<QubitRegister>(program);
    instance.bit_register_ = std::make_shared<BitRegister>(program);
}

[[nodiscard]] /* static */ RegisterManager& RegisterManager::get_instance() {
    auto& instance = get_instance_impl();
    if (!instance.qubit_register_ or !instance.bit_register_) {
        throw RegisterManagerError{ "uninitialized register manager" };
    }
    return instance;
}

[[nodiscard]] std::size_t RegisterManager::get_qubit_register_size() const {
    return qubit_register_->size();
}

[[nodiscard]] std::size_t RegisterManager::get_bit_register_size() const {
    return bit_register_->size();
}

[[nodiscard]] Range RegisterManager::get_qubit_range(const VariableName& name) const {
    return qubit_register_->at(name);
}

[[nodiscard]] Range RegisterManager::get_bit_range(const VariableName& name) const {
    return bit_register_->at(name);
}

[[nodiscard]] Index RegisterManager::get_qubit_index(
    const VariableName& name, const std::optional<Index>& sub_index) const {
    return qubit_register_->at(name, sub_index);
}

[[nodiscard]] Index RegisterManager::get_bit_index(
    const VariableName& name, const std::optional<Index>& sub_index) const {
    return bit_register_->at(name, sub_index);
}

[[nodiscard]] VariableName RegisterManager::get_qubit_variable_name(const std::size_t& index) const {
    return qubit_register_->at(index);
}

[[nodiscard]] VariableName RegisterManager::get_bit_variable_name(const std::size_t& index) const {
    return bit_register_->at(index);
}

[[nodiscard]] Index RegisterManager::get_qubit_variable_index(const Index& index) const {
    const auto& variable_name = qubit_register_->at(index);
    const auto& first_index = qubit_register_->at(variable_name).first;
    assert(index >= first_index);
    return index - first_index;
}

[[nodiscard]] Index RegisterManager::get_bit_variable_index(const Index& index) const {
    const auto& variable_name = bit_register_->at(index);
    const auto& first_index = bit_register_->at(variable_name).first;
    assert(index >= first_index);
    return index - first_index;
}

[[nodiscard]] std::shared_ptr<QubitRegister> RegisterManager::get_qubit_register() const {
    return qubit_register_;
}

[[nodiscard]] std::shared_ptr<BitRegister> RegisterManager::get_bit_register() const {
    return bit_register_;
}

[[nodiscard]] bool RegisterManager::is_dirty_qubit(const Index& index) const {
    return qubit_register_->is_dirty(index);
}

[[nodiscard]] bool RegisterManager::is_dirty_bit(const Index& index) const {
    return bit_register_->is_dirty(index);
}

void RegisterManager::set_dirty_qubit(const Index& index) {
    qubit_register_->set_dirty(index);
}

void RegisterManager::set_dirty_bit(const Index& index) {
    qubit_register_->set_dirty(index);
}

std::ostream& operator<<(std::ostream& os, const Range& range) {
    return os << fmt::format(
               "[{}{}]", range.first, range.size == 1 ? "" : fmt::format("..{}", range.first + range.size - 1));
}

std::ostream& operator<<(std::ostream& os, const QubitRegister& qubit_register) {
    return os << qubit_register.to_string();
}

std::ostream& operator<<(std::ostream& os, const BitRegister& bit_register) {
    return os << bit_register.to_string();
}

}  // namespace qx
