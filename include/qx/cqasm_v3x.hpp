#pragma once

#include "v3x/cqasm-semantic-gen.hpp"
#include "v3x/cqasm-primitives.hpp"
#include "v3x/cqasm-types.hpp"
#include "v3x/cqasm-types-gen.hpp"
#include "v3x/cqasm-values.hpp"
#include "v3x/cqasm-values-gen.hpp"


namespace qx {

namespace v3_ast = cqasm::v3x::ast;
namespace v3_primitives = cqasm::v3x::primitives;
namespace v3_semantic = cqasm::v3x::semantic;
namespace v3_tree = ::cqasm::tree;
namespace v3_types = cqasm::v3x::types;
namespace v3_values = cqasm::v3x::values;

template <typename T> using V3Many = v3_ast::Many<T>;
using V3ConstInt = v3_values::ConstInt;
using V3Instruction = v3_semantic::Instruction;
using V3Node = v3_semantic::Node;
using V3OneProgram = v3_tree::One<v3_semantic::Program>;
using V3OneVariable = v3_tree::One<v3_semantic::Variable>;
using V3RecursiveVisitor = v3_semantic::RecursiveVisitor;
using V3Type = v3_types::Type;
using V3Value = v3_values::Value;
using V3Variable = v3_semantic::Variable;

bool is_qubit_variable(const V3Variable &variable);
bool is_bit_variable(const V3Variable &variable);

}  // namespace qx
