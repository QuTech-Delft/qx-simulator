#pragma once

#include "v3x/cqasm-analysis-result.hpp"
#include "v3x/cqasm-primitives.hpp"
#include "v3x/cqasm-semantic-gen.hpp"
#include "v3x/cqasm-types-gen.hpp"
#include "v3x/cqasm-types.hpp"
#include "v3x/cqasm-values-gen.hpp"
#include "v3x/cqasm-values.hpp"

namespace qx {

namespace cqasm_tree = ::cqasm::tree;
namespace cqasm_v3x_analyzer = cqasm::v3x::analyzer;
namespace cqasm_v3x_ast = cqasm::v3x::ast;
namespace cqasm_v3x_primitives = cqasm::v3x::primitives;
namespace cqasm_v3x_semantic = cqasm::v3x::semantic;
namespace cqasm_v3x_types = cqasm::v3x::types;
namespace cqasm_v3x_values = cqasm::v3x::values;

template <typename T>
using CqasmV3xAny = cqasm_v3x_ast::Any<T>;
template <typename T>
using CqasmV3xMany = cqasm_v3x_ast::Many<T>;
template <typename T>
using TreeOne = cqasm_tree::One<T>;

using CqasmV3xApiVersion = cqasm_v3x_primitives::Version;
using CqasmV3xAnalysisResult = cqasm_v3x_analyzer::AnalysisResult;
using CqasmV3xBitType = cqasm_v3x_types::Bit;
using CqasmV3xBlock = cqasm_v3x_semantic::Block;
using CqasmV3xConstInt = cqasm_v3x_values::ConstInt;
using CqasmV3xIndices = CqasmV3xMany<CqasmV3xConstInt>;
using CqasmV3xInstruction = cqasm_v3x_semantic::Instruction;
using CqasmV3xNode = cqasm_v3x_semantic::Node;
using CqasmV3xProgram = cqasm_v3x_semantic::Program;
using CqasmV3xQubitType = cqasm_v3x_types::Qubit;
using CqasmV3xRecursiveVisitor = cqasm_v3x_semantic::RecursiveVisitor;
using CqasmV3xType = cqasm_v3x_types::Type;
using CqasmV3xValue = cqasm_v3x_values::Value;
using CqasmV3xVariable = cqasm_v3x_semantic::Variable;
using CqasmV3xVariables = CqasmV3xAny<CqasmV3xVariable>;
using CqasmV3xVersion = cqasm_v3x_semantic::Version;

bool is_qubit_variable(const CqasmV3xVariable& variable);
bool is_bit_variable(const CqasmV3xVariable& variable);

}  // namespace qx
