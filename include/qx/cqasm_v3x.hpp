#pragma once

#include "libqasm/v3x/analysis_result.hpp"
#include "libqasm/v3x/primitives.hpp"
#include "libqasm/v3x/semantic_generated.hpp"
#include "libqasm/v3x/types.hpp"
#include "libqasm/v3x/types_generated.hpp"
#include "libqasm/v3x/values.hpp"
#include "libqasm/v3x/values_generated.hpp"

namespace qx {

namespace cqasm_tree = ::cqasm::tree;
namespace cqasm_v3x_analyzer = cqasm::v3x::analyzer;
namespace cqasm_v3x_primitives = cqasm::v3x::primitives;
namespace cqasm_v3x_semantic = cqasm::v3x::semantic;
namespace cqasm_v3x_syntactic = cqasm::v3x::syntactic;
namespace cqasm_v3x_types = cqasm::v3x::types;
namespace cqasm_v3x_values = cqasm::v3x::values;

template <typename T>
using CqasmV3xAny = cqasm_v3x_syntactic::Any<T>;
template <typename T>
using CqasmV3xMany = cqasm_v3x_syntactic::Many<T>;
template <typename T>
using CqasmV3xMaybe = cqasm_v3x_syntactic::Maybe<T>;
template <typename T>
using TreeLink = cqasm::tree::Link<T>;
template <typename T>
using TreeOne = cqasm_tree::One<T>;

using CqasmV3xAnalysisResult = cqasm_v3x_analyzer::AnalysisResult;
using CqasmV3xApiVersion = cqasm_v3x_primitives::Version;
using CqasmV3xBitType = cqasm_v3x_types::Bit;
using CqasmV3xBlock = cqasm_v3x_semantic::Block;
using CqasmV3xConstInt = cqasm_v3x_values::ConstInt;
using CqasmV3xGate = cqasm_v3x_semantic::Gate;
using CqasmV3xGateInstruction = cqasm_v3x_semantic::GateInstruction;
using CqasmV3xIndexRef = cqasm_v3x_values::IndexRef;
using CqasmV3xIndices = CqasmV3xMany<CqasmV3xConstInt>;
using CqasmV3xInstruction = cqasm_v3x_semantic::Instruction;
using CqasmV3xNode = cqasm_v3x_semantic::Node;
using CqasmV3xNonGateInstruction = cqasm_v3x_semantic::NonGateInstruction;
using CqasmV3xProgram = cqasm_v3x_semantic::Program;
using CqasmV3xQubitType = cqasm_v3x_types::Qubit;
using CqasmV3xRecursiveVisitor = cqasm_v3x_semantic::RecursiveVisitor;
using CqasmV3xType = cqasm_v3x_types::Type;
using CqasmV3xTypeBase = cqasm_v3x_types::TypeBase;
using CqasmV3xValue = cqasm_v3x_values::Value;
using CqasmV3xValueBase = cqasm_v3x_values::ValueBase;
using CqasmV3xVariable = cqasm_v3x_semantic::Variable;
using CqasmV3xVariableLink = TreeLink<CqasmV3xVariable>;
using CqasmV3xVariableRef = cqasm_v3x_values::VariableRef;
using CqasmV3xVersion = cqasm_v3x_semantic::Version;

using CqasmV3xParameters = CqasmV3xAny<CqasmV3xValueBase>;
using CqasmV3xOperand = CqasmV3xValueBase;
using CqasmV3xOperandIndices = CqasmV3xAny<CqasmV3xConstInt>;
using CqasmV3xOperands = CqasmV3xAny<CqasmV3xValueBase>;
using CqasmV3xVariables = CqasmV3xAny<CqasmV3xVariable>;

bool is_qubit_variable(const CqasmV3xVariable& variable);
bool is_bit_variable(const CqasmV3xVariable& variable);

}  // namespace qx
