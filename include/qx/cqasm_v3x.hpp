#pragma once

#include "v3x/cqasm.hpp"
#include "v3x/cqasm-semantic-gen.hpp"
#include "v3x/cqasm-primitives.hpp"
#include "v3x/cqasm-types.hpp"
#include "v3x/cqasm-types-gen.hpp"
#include "v3x/cqasm-values.hpp"
#include "v3x/cqasm-values-gen.hpp"


namespace qx {

namespace cqasm_v3x_analyzer = cqasm::v3x::analyzer;
namespace cqasm_v3x_ast = cqasm::v3x::ast;
namespace cqasm_v3x_primitives = cqasm::v3x::primitives;
namespace cqasm_v3x_semantic = cqasm::v3x::semantic;
namespace cqasm_v3x_tree = ::cqasm::tree;
namespace cqasm_v3x_types = cqasm::v3x::types;
namespace cqasm_v3x_values = cqasm::v3x::values;

using CqasmV3xAnalysisResult = cqasm_v3x_analyzer::AnalysisResult;
template <typename T>
using CqasmV3xMany = cqasm_v3x_ast::Many<T>;
using CqasmV3xConstInt = cqasm_v3x_values::ConstInt;
using CqasmV3xIndices = CqasmV3xMany<CqasmV3xConstInt>;
using CqasmV3xInstruction = cqasm_v3x_semantic::Instruction;
using CqasmV3xNode = cqasm_v3x_semantic::Node;
using CqasmV3xProgram = cqasm_v3x_semantic::Program;
using CqasmV3xRecursiveVisitor = cqasm_v3x_semantic::RecursiveVisitor;
using CqasmV3xType = cqasm_v3x_types::Type;
using CqasmV3xValue = cqasm_v3x_values::Value;
using CqasmV3xVariable = cqasm_v3x_semantic::Variable;
template <typename T>
using TreeOne = cqasm_v3x_tree::One<T>;

bool is_qubit_variable(const CqasmV3xVariable &variable);
bool is_bit_variable(const CqasmV3xVariable &variable);

}  // namespace qx
